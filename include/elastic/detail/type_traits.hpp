#pragma once
#include <elastic/detail/config.hpp>
#include <forward_list>
#include <list>
#include <string>
#include <vector>

namespace elastic
{
	namespace detail
	{
		struct any_type
		{
			std::size_t ignore_;

			template <typename _Ty>
			constexpr operator _Ty() const noexcept;
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
		struct is_sequence<std::vector<_Ty>> : std::true_type
		{};

		template <typename _Ty>
		struct is_sequence<std::list<_Ty>> : std::true_type
		{};

		template <typename _Ty>
		struct is_sequence<std::forward_list<_Ty>> : std::true_type
		{};

		template <typename _Ty>
		struct is_sequence<std::basic_string<_Ty>> : std::true_type
		{};

		template <typename _Ty, std::size_t N>
		struct is_sequence<std::array<_Ty, N>> : std::true_type
		{};

		template <typename _Ty, std::size_t N>
		struct is_sequence<_Ty[N]> : std::true_type
		{};

		template <typename _Ty>
		struct is_sequence<_Ty[]> : std::true_type
		{};

		template <typename _Ty>
		struct identify
		{
			using type = _Ty;
		};

		template <typename _Ty>
		using identify_t = typename identify<std::remove_cvref_t<_Ty>>::type;

	} // namespace detail
} // namespace elastic