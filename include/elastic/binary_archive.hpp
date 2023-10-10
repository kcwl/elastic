#pragma once
#include "binary_primitive.hpp"
#include "binary_serialize.hpp"
#include "interface_archive.hpp"
#include "interface_archive.hpp"

#include <istream>

namespace elastic
{
	class binary_iarchive : public binary_iprimitive<binary_iarchive, char, std::char_traits<char>>,
							public interface_iarchive<binary_iarchive>
	{
	public:
		template <typename _StreamBuffer>
		requires(std::is_base_of_v<std::streambuf, _StreamBuffer>)
		explicit binary_iarchive(_StreamBuffer& bs)
			: binary_iprimitive<binary_iarchive, char, std::char_traits<char>>(bs)
		{}

		binary_iarchive(std::istream& is)
			: binary_iprimitive<binary_iarchive, char, std::char_traits<char>>(*is.rdbuf())
		{}

	public:
		template <typename _Ty>
		void load_override(_Ty& t)
		{
			try
			{
				this->start();

				archive::binary_load(*this, t);
			}
			catch (...)
			{
				this->roll_back();

				_Ty error{};

				std::swap(t, error);
			}
		}
	};

	class binary_oarchive : public binary_oprimitive<binary_oarchive, char, std::char_traits<char>>,
							public interface_oarchive<binary_oarchive>
	{
	public:
		template <typename _StreamBuffer>
		requires(std::is_base_of_v<std::streambuf, _StreamBuffer>)
		explicit binary_oarchive(_StreamBuffer& bsb)
			: binary_oprimitive<binary_oarchive, char, std::char_traits<char>>(bsb)
		{}

		binary_oarchive(std::ostream& os)
			: binary_oprimitive<binary_oarchive, char, std::char_traits<char>>(*os.rdbuf())
		{}

	public:
		template <typename _Ty>
		void save_override(_Ty&& t)
		{
			archive::binary_save(*this, std::forward<_Ty>(t));
		}
	};

} // namespace elastic