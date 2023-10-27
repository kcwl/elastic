#pragma once
#include "type_traits.hpp"

#include <cstdint>
#include <utility>

namespace elastic
{
	template <typename _Ty, std::size_t N>
	concept aggregate_inialize = is_aggregate_initialize<_Ty, N>::value;

	template <typename _Ty>
	concept copable = std::is_copy_constructible_v<std::remove_all_extents_t<_Ty>> &&
					  std::is_move_constructible_v<std::remove_all_extents_t<_Ty>> &&
					  std::is_move_constructible_v<std::remove_all_extents_t<_Ty>>;

	template <typename _Ty>
	concept reflection = requires {
		copable<_Ty> && !std::is_polymorphic_v<_Ty> && (std::is_aggregate_v<_Ty> || std::is_scalar_v<_Ty>);
	};

	template <typename _Ty>
	concept tuple_t = requires() { std::tuple_size<_Ty>(); };

	template <typename _Ty>
	concept class_t = std::is_class_v<std::remove_reference_t<_Ty>>;

	template <typename _Ty>
	concept signed_numric_v = is_any_of_v<_Ty, int8_t, int16_t, int32_t, int64_t>;

	template<typename _Ty>
	concept unsigned_numric_v = std::is_unsigned_v<std::remove_cvref_t<_Ty>>;

	template <typename _Ty>
	concept other_numric_v =
		is_any_of_v<std::remove_cvref_t<_Ty>, bool, std::byte, char> || std::is_enum_v<std::remove_cvref_t<_Ty>> || unsigned_numric_v<_Ty>;

	template <typename _Ty>
	concept multi_numric_v = is_any_of_v<_Ty, double, float>;

	template <typename _Ty>
	concept varint_t = signed_numric_v<std::remove_cvref_t<_Ty>> || other_numric_v<std::remove_cvref_t<_Ty>>;

	template <typename _Ty>
	concept integer_t = signed_numric_v<_Ty> || other_numric_v<_Ty> || multi_numric_v<_Ty>;

	template <typename _Ty>
	concept fixed_v = is_fixed<_Ty>::value || multi_numric_v<_Ty>;

	template <typename _Ty>
	concept optional_t = requires(_Ty value) {
		value.has_value();
		*value;
	};

	template <typename _Ty>
	concept pod_t = std::is_trivial_v<std::remove_cvref_t<_Ty>> &&
					std::is_standard_layout_v<std::remove_cvref_t<_Ty>> && !integer_t<std::remove_cvref_t<_Ty>>;

	template <typename _Ty>
	concept sequence_t = requires(_Ty value) {
		value.size();
		std::begin(value);
		std::end(value);
		typename std::remove_cvref_t<_Ty>::value_type;
	};

	template <typename _Ty>
	concept non_inherit_t = integer_t<_Ty> || pod_t<_Ty> || sequence_t<_Ty> || optional_t<_Ty> || fixed_v<_Ty>;

	template <typename _Ty>
	concept inherit_t = !non_inherit_t<_Ty>;

} // namespace elastic