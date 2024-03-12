#pragma once
#include <forward_list>
#include <list>
#include <map>
#include <string>
#include <type_traits>
#include <vector>

namespace elastic
{
	template<typename _Ty>
	concept float_point_t = std::is_floating_point_v<std::remove_reference_t<_Ty>>;

	template <typename _Ty>
	concept integer_t = std::is_integral_v<std::remove_reference_t<_Ty>>;

	namespace detail
	{
		template<typename _Ty>
		struct zig_zag
		{
			using type = _Ty;
		};

		template<>
		struct zig_zag<int8_t>
		{
			using type = uint8_t;
		};

		template<>
		struct zig_zag<char>
		{
			using type = uint8_t;
		};

		template<>
		struct zig_zag<bool>
		{
			using type = uint8_t;
		};

		template<>
		struct zig_zag<int16_t>
		{
			using type = uint16_t;
		};

		template<>
		struct zig_zag<int32_t>
		{
			using type = uint32_t;
		};

		template<>
		struct zig_zag<int64_t>
		{
			using type = uint64_t;
		};

		template<typename _Ty>
		struct remove_unsigned
		{
			using type = _Ty;
		};

		template<>
		struct remove_unsigned<uint8_t>
		{
			using type = int8_t;
		};

		template<>
		struct remove_unsigned<uint16_t>
		{
			using type = int16_t;
		};

		template<>
		struct remove_unsigned<uint32_t>
		{
			using type = int32_t;
		};

		template<>
		struct remove_unsigned<uint64_t>
		{
			using type = int64_t;
		};
	}

	template<typename _Ty>
	using zig_zag_t = typename detail::zig_zag<std::remove_cvref_t<_Ty>>::type;

	template<typename _Ty>
	using remove_unsigned_t = typename detail::remove_unsigned<std::remove_cvref_t<_Ty>>::type;

	template <typename _Ty, typename... _Args>
	inline constexpr bool is_any_of_v = std::disjunction_v<std::is_same<std::remove_cvref_t<_Ty>, _Args>...>;

	template <typename _Ty>
	concept copable_t = std::is_copy_constructible_v<std::remove_all_extents_t<_Ty>> &&
						std::is_move_constructible_v<std::remove_all_extents_t<_Ty>>;

	template <typename _Ty>
	concept reflection_t =
		copable_t<_Ty> && !std::is_polymorphic_v<_Ty> && (std::is_aggregate_v<_Ty> || std::is_scalar_v<_Ty>);

	template <typename _Ty>
	concept tuple_t = requires() { std::tuple_size<_Ty>(); };

	template <typename _Ty>
	concept class_t = std::is_class_v<std::remove_reference_t<_Ty>>;

	template <typename _Ty>
	concept optional_t = requires(_Ty value) {
		value.has_value();
		*value;
	};

	template<typename _Ty>
	concept pod_and_integer_t = std::is_trivial_v<std::remove_cvref_t<_Ty>> && std::is_standard_layout_v<std::remove_cvref_t<_Ty>>;

	template <typename _Ty>
	concept pod_t = class_t<_Ty> && pod_and_integer_t<_Ty> && !integer_t<_Ty>;


	template <typename _Ty>
	concept sequence_t = requires(_Ty value) {
		value.size();
		std::begin(value);
		std::end(value);
		value.data();
		typename std::remove_cvref_t<_Ty>::value_type;
	};

	template <typename _Ty>
	struct is_map : public std::false_type
	{};

	template <typename _Key, typename _Value>
	struct is_map<std::map<_Key, _Value>> : std::true_type
	{};

	template <typename _Ty>
	concept map_t = is_map<_Ty>::value;

	template <typename _Ty>
	concept non_inherit_t = integer_t<_Ty> || pod_t<_Ty> || sequence_t<_Ty> || optional_t<_Ty> || map_t<_Ty> || float_point_t<_Ty>;

	template <typename _Ty>
	concept inherit_t = !non_inherit_t<_Ty>;

	template <typename _Ty>
	concept swap_t = requires(_Ty value) { value.swap(value); };

} // namespace elastic