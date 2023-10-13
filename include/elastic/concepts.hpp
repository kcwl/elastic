#pragma once
#include "property.hpp"
#include "type_traits.hpp"

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
	concept class_cpt = std::is_class_v<std::remove_reference_t<_Ty>>;

	template <typename _Ty>
	concept single_signed_numric =
		std::is_same_v<_Ty, char> || std::is_same_v<_Ty, int8_t> || std::is_same_v<_Ty, int16_t> ||
		std::is_same_v<_Ty, int32_t> || std::is_same_v<_Ty, int64_t> || std::is_enum_v<_Ty> || std::same_as<_Ty, bool>;

	template <typename _Ty>
	concept single_unsigned_numric = std::is_same_v<_Ty, uint8_t> || std::is_same_v<_Ty, uint16_t> ||
									 std::is_same_v<_Ty, uint32_t> || std::is_same_v<_Ty, uint64_t>;

	template <typename _Ty>
	concept single_numric =
		single_signed_numric<std::remove_cvref_t<_Ty>> || single_unsigned_numric<std::remove_cvref_t<_Ty>>;

	template <typename _Ty>
	concept multi_numric =
		std::is_same_v<std::remove_cvref_t<_Ty>, double> || std::is_same_v<std::remove_cvref_t<_Ty>, float>;

	template <typename _Ty>
	concept varint_t = single_signed_numric<std::remove_cvref_t<_Ty>> ||
					   single_unsigned_numric<std::remove_cvref_t<_Ty>> || multi_numric<std::remove_cvref_t<_Ty>>;

	template <typename _Ty>
	concept pod_t = std::is_trivial_v<std::remove_cvref_t<_Ty>> && std::is_standard_layout_v<std::remove_cvref_t<_Ty>>;

	template <typename _Ty>
	concept positive_integar = requires(_Ty value) {
		value >= 0;
		!multi_numric<_Ty>;
	};

	template <typename _Ty>
	concept negative_integar = requires { !positive_integar<_Ty>&& single_signed_numric<_Ty>; };

	template <typename _Ty>
	concept sequence_t = requires(_Ty value) {
		is_sequence<std::remove_cvref_t<_Ty>>::value;
		value.size();
		std::begin(value);
		std::end(value);
		typename std::remove_cvref_t<_Ty>::value_type;
	};

	template <typename _Ty>
	concept integer_like = std::convertible_to<_Ty, uint64_t>;

	template <typename _Ty>
	concept signed_integer = is_any_of_v<_Ty, int8_t, int16_t, int32_t, int64_t>;

	template <typename _Ty>
	concept char_t =
		std::is_same_v<std::remove_cvref_t<_Ty>, char> || std::is_same_v<std::remove_cvref_t<_Ty>, wchar_t>;

	template <typename _Ty>
	concept string_t = std::is_same_v<_Ty, std::string>;

	template <typename _Ty>
	concept fixed_t = is_fixed<_Ty>::value;

	template <typename _Ty>
	concept optional_t = requires(_Ty value) {
		value.has_value();
		*value;
	};

	template <typename _Ty>
	concept length_body_parse_t = string_t<std::remove_cvref<_Ty>> || sequence_t<_Ty>;

	template <typename _Ty>
	concept fix_parse_t = std::convertible_to<std::remove_cvref<_Ty>, uint64_t>;

	template <typename _Ty>
	concept varint_prase_t = varint_t<_Ty>;
} // namespace elastic