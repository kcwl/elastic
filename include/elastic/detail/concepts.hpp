#pragma once
#include <elastic/detail/type_traits.hpp>
#include <string>
#include <utility>

namespace elastic
{
	namespace detail
	{
		template <typename _Ty, std::size_t N>
		concept aggregate_inialize = is_aggregate_initialize<_Ty, N>::value;

		template <typename _Ty>
		concept copable = std::is_copy_constructible_v<std::remove_all_extents_t<_Ty>> &&
						  std::is_move_constructible_v<std::remove_all_extents_t<_Ty>> &&
						  std::is_move_constructible_v<std::remove_all_extents_t<_Ty>>;

		template <typename _Ty>
		concept reflection =
			requires {
				copable<_Ty> && !std::is_polymorphic_v<_Ty> && (std::is_aggregate_v<_Ty> || std::is_scalar_v<_Ty>);
			};

		template <typename _Ty>
		concept tuple = requires() { std::tuple_size<_Ty>(); };

		template <typename _Ty>
		concept class_cpt = std::is_class_v<_Ty>;

		template <typename _Ty>
		concept single_signed_numric =
			std::is_same_v<_Ty, char> || std::is_same_v<_Ty, int8_t> || std::is_same_v<_Ty, int16_t> ||
			std::is_same_v<_Ty, int32_t> || std::is_same_v<_Ty, int64_t>;

		template <typename _Ty>
		concept single_unsigned_numric = std::is_same_v<_Ty, uint8_t> || std::is_same_v<_Ty, uint16_t> ||
										 std::is_same_v<_Ty, uint32_t> || std::is_same_v<_Ty, uint64_t>;

		template <typename _Ty>
		concept single_numric = single_signed_numric<_Ty> || single_unsigned_numric<_Ty>;

		template <typename _Ty>
		concept multi_numric =
			std::is_same_v<std::remove_cvref_t<_Ty>, double> || std::is_same_v<std::remove_cvref_t<_Ty>, float>;

		template <typename _Ty>
		concept varint = single_signed_numric<_Ty> || single_unsigned_numric<_Ty> || multi_numric<_Ty>;

		template <typename _Ty>
		concept pod =
			std::is_trivial_v<std::remove_cvref_t<_Ty>> && std::is_standard_layout_v<std::remove_cvref_t<_Ty>>;

		template <typename _Ty>
		concept positive_integar = requires(_Ty value) {
									   value >= 0;
									   !multi_numric<_Ty>;
								   };

		template <typename _Ty>
		concept negative_integar = requires { !positive_integar<_Ty>&& single_signed_numric<_Ty>; };

		template <typename _Ty>
		concept string_t = std::is_same_v<std::remove_cvref_t<_Ty>, std::string> || is_vector<_Ty>::value;

	} // namespace detail
} // namespace elastic