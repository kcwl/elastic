#pragma once
#include "primitive.hpp"
#include "serialize.hpp"

namespace elastic
{
	class binary_iarchive : public detail::binary_iprimitive<binary_iarchive, uint8_t>
	{
	public:
		template <typename _StreamBuffer>
		explicit binary_iarchive(_StreamBuffer& bs)
			: binary_iprimitive<binary_iarchive, uint8_t, std::char_traits<uint8_t>>(bs)
		{}

	public:
		template <typename _Ty>
		binary_iarchive& operator>>(_Ty& t)
		{
			load_override(t);

			return *this;
		}

		template <typename _Ty>
		binary_iarchive& operator&(_Ty& t)
		{
			return operator>> (t);
		}

		void consume(std::streamoff off)
		{
			return this->streambuf_.consume(off);
		}

	private:
		template <typename _Ty>
		bool load_override(_Ty& t)
		{
			bool result = this->transcation([&] { binary::template deserialize(*this, t); });

			if (!result)
			{
				t = _Ty{};
			}

			return result;
		}
	};

	class binary_oarchive : public detail::binary_oprimitive<binary_oarchive, uint8_t>
	{
	public:
		template <typename _StreamBuffer>
		explicit binary_oarchive(_StreamBuffer& bsb)
			: binary_oprimitive<binary_oarchive, uint8_t, std::char_traits<uint8_t>>(bsb)
		{}

	public:
		template <typename _Ty>
		binary_oarchive& operator<<(_Ty&& t)
		{
			save_override(std::forward<_Ty>(t));

			return *this;
		}

		template <typename _Ty>
		binary_oarchive& operator&(_Ty&& t)
		{
			return  operator<< (std::forward<_Ty>(t));
		}

		void commit(std::streamoff off)
		{
			return this->streambuf_.commit(off);
		}

	private:
		template <typename _Ty>
		void save_override(_Ty&& t)
		{
			this->transcation([&] { binary::template serialize(*this, t); });
		}
	};
} // namespace elastic