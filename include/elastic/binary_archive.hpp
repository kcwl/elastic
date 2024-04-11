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
			primitive_guard lk(*this);

			try
			{
				binary::deserialize(*this, t);
			}
			catch (...)
			{
				this->failed();
			}

			return *this;
		}

		template <typename _Ty>
		binary_iarchive& operator&(_Ty& t)
		{
			return operator>>(t);
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
			primitive_guard lk(*this);

			try
			{
				binary::template serialize(*this, std::forward<_Ty>(t));
			}
			catch (...)
			{
				this->failed();
			}

			return *this;
		}

		template <typename _Ty>
		binary_oarchive& operator&(_Ty&& t)
		{
			return operator<<(std::forward<_Ty>(t));
		}
	};
} // namespace elastic