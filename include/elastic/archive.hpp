#pragma once
#include "interface.hpp"
#include "primitive.hpp"
#include "serialize.hpp"

#include <istream>

namespace elastic
{
	class binary_iarchive : public binary_iprimitive<binary_iarchive, char, std::char_traits<char>>,
							public interface_iarchive<binary_iarchive>
	{
		friend class interface_iarchive<binary_iarchive>;

	public:
		template <typename _StreamBuffer>
		requires(std::is_base_of_v<std::streambuf, _StreamBuffer>)
		explicit binary_iarchive(_StreamBuffer& bs)
			: binary_iprimitive<binary_iarchive, char, std::char_traits<char>>(bs)
			, has_first_archive_(false)
		{}

		binary_iarchive(std::istream& is)
			: binary_iprimitive<binary_iarchive, char, std::char_traits<char>>(*is.rdbuf())
			, has_first_archive_(false)
		{}

	private:
		template <typename _Ty>
		bool load_override(_Ty& t)
		{
			bool result = true;

			try
			{
				has_first_archive_ = this->transfer();

				binary::template deserialize(*this, t);
			}
			catch (std::exception& ec)
			{
				if (!has_first_archive_)
					throw ec;

				this->roll_back();

				_Ty error{};

				std::swap(t, error);

				result = false;
			}

			return result;
		}

	private:
		bool has_first_archive_;
	};

	class binary_oarchive : public binary_oprimitive<binary_oarchive, char, std::char_traits<char>>,
							public interface_oarchive<binary_oarchive>
	{
		friend class interface_oarchive<binary_oarchive>;

	public:
		template <typename _StreamBuffer>
		requires(std::is_base_of_v<std::streambuf, _StreamBuffer>)
		explicit binary_oarchive(_StreamBuffer& bsb)
			: binary_oprimitive<binary_oarchive, char, std::char_traits<char>>(bsb)
		{}

		binary_oarchive(std::ostream& os)
			: binary_oprimitive<binary_oarchive, char, std::char_traits<char>>(*os.rdbuf())
		{}

	private:
		template <typename _Ty>
		void save_override(_Ty&& t)
		{
			binary::template serialize(*this, std::forward<_Ty>(t));
		}
	};

	template <typename _Ty, typename _Buffer>
	bool to_binary(_Ty&& t, _Buffer& buffer)
	{
		binary_oarchive oa(buffer);

		oa << std::forward<_Ty>(t);

		return oa.good();
	}

	template <typename _Ty, typename _Buffer>
	bool from_binary(_Ty& t, _Buffer& buffer)
	{
		binary_iarchive ia(buffer);

		ia >> t;

		return ia.good();
	}

} // namespace elastic