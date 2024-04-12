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

		virtual ~binary_iarchive() = default;

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
			{}

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

		virtual ~binary_oarchive() = default;

	public:
		template <typename _Ty>
		binary_oarchive& operator<<(const _Ty& t)
		{
			primitive_guard lk(*this);

			try
			{
				binary::template serialize(*this, t);
			}
			catch (...){}

			return *this;
		}

		template <typename _Ty>
		binary_oarchive& operator&(const _Ty& t)
		{
			return operator<<(t);
		}
	};
} // namespace elastic