#pragma once
#include <elastic/detail/config.hpp>

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
        {
        };

        template <typename _Ty, size_t... I>
        struct is_aggregate_initalize_impl<_Ty, std::index_sequence<I...>, std::void_t<decltype(_Ty{any_type{I}...})>>
            : std::true_type
        {
        };

        template <typename _Ty, std::size_t N>
        struct is_aggregate_initialize : is_aggregate_initalize_impl<_Ty, std::make_index_sequence<N>>
        {
        };
    } // namespace detail
} // namespace elastic