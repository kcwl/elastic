#pragma once
#include <forward_list>
#include <list>
#include <string>
#include <type_traits>
#include <vector>

namespace elastic
{
	namespace detail
	{
		struct any_type
		{
			std::size_t ignore_;

			template <typename _Ty>
			constexpr operator _Ty() const noexcept {};
		};

		template <typename _Ty, typename Indices, typename = void>
		struct is_aggregate_initalize_impl : std::false_type
		{};

		template <typename _Ty, size_t... I>
		struct is_aggregate_initalize_impl<_Ty, std::index_sequence<I...>,
										   std::void_t<decltype(_Ty{ any_type{ I }... })>> : std::true_type
		{};

		template <typename _Ty, std::size_t N>
		struct is_aggregate_initialize : is_aggregate_initalize_impl<_Ty, std::make_index_sequence<N>>
		{};

		template <typename _Ty>
		struct is_sequence : std::false_type
		{};

		template <typename _Ty>
		struct is_sequence<std::list<_Ty>> : std::true_type
		{};

		template <typename _Ty>
		struct is_sequence<std::forward_list<_Ty>> : std::true_type
		{};

		//template <typename _Ty>
		//struct is_sequence<std::basic_string<_Ty>> : std::true_type
		//{};

		

		template <typename _Ty>
		struct identify
		{
			using type = _Ty;
		};

		template <typename _Ty>
		using identify_t = typename identify<std::remove_cvref_t<_Ty>>::type;

		template <typename _Ty>
		struct relative
		{
			using type = _Ty;
		};

		template <>
		struct relative<int8_t>
		{
			using type = uint8_t;
		};

		template <>
		struct relative<int16_t>
		{
			using type = uint16_t;
		};

		template <>
		struct relative<int32_t>
		{
			using type = uint32_t;
		};

		template <>
		struct relative<int64_t>
		{
			using type = uint64_t;
		};

		template <typename _Ty>
		struct is_fixed : std::false_type
		{};

		template <typename _Ty>
		struct is_fixed<fixed<_Ty>> : std::true_type
		{};

		template<typename _Ty>
		struct is_string : std::false_type
		{};

		template<>
		struct is_string<std::string> : std::true_type{};

		template <typename _Ty, std::size_t N>
		struct is_string<std::array<_Ty, N>> : std::true_type
		{};

		template <typename _Ty, std::size_t N>
		struct is_string<_Ty[N]> : std::true_type
		{};

		template <typename _Ty>
		struct is_string<_Ty[]> : std::true_type
		{};

		template <typename _Ty>
		struct is_string<std::vector<_Ty>> : std::true_type
		{};

		template<typename _Ty>
		constexpr static bool is_string_v = is_string<_Ty>::value;

		template<typename _Ty, typename... _Args>
		constexpr static bool is_any_of_v = std::disjunction_v<std::is_same_v<_Ty, _Args>...>;

	} // namespace detail
} // namespace elastic