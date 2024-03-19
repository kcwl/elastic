#pragma once
#include "type_traits.hpp"

namespace elastic
{
	inline static constexpr int32_t zig_zag_bit = 7;

	template <elastic::integer_t _Ty>
	elastic::zig_zag_t<_Ty> zigzag_encode(_Ty value)
	{
		using value_type = elastic::zig_zag_t<_Ty>;

		using remove_unsigned_type = elastic::remove_unsigned_t<_Ty>;

		constexpr auto size = sizeof(_Ty) * 8 - 1;

		return static_cast<remove_unsigned_type>(value) << 1 ^ static_cast<remove_unsigned_type>(value) >> size;
	}

	template <elastic::integer_t _Ty>
	_Ty zigzag_decode(elastic::zig_zag_t<_Ty> value)
	{
		return static_cast<_Ty>((value >> 1) ^ (~(value & 1) + 1));
	}
}