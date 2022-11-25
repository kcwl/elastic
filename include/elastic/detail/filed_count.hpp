#pragma once
#include <elastic/detail/size_t_.hpp>
#include <elastic/detail/concepts.hpp>

namespace elastic
{
	namespace detail
	{
		using one_range = std::true_type;
		using multi_range = std::false_type;

		template<std::size_t Begin, std::size_t End>
		using is_one_element_range = std::bool_constant<Begin == End>;

		template<typename _Ty, std::size_t Start, std::size_t End>
		constexpr std::size_t detect_fields_greey(one_range)
		{
			static_assert(Start == End, "It is not a correct way with binary search!");

			if constexpr (detail::aggregate_inialize<_Ty, Start>)
			{
				return Start;
			}
			else
			{
				return 0;
			}
		}

		template<typename _Ty, std::size_t Begin, std::size_t End>
		constexpr std::size_t detect_fields_greey(multi_range);

		template<typename _Ty, std::size_t Begin, std::size_t End>
		requires(aggregate_inialize<_Ty,End>)
		constexpr auto detect_fields_greey(multi_range)
		{
			constexpr std::size_t next = End + (End - Begin + 1) / 2;

			return detail::template detect_fields_greey<_Ty, End, next>(is_one_element_range<End, next>{});
		}

		template<typename T, std::size_t Begin, std::size_t End>
		constexpr std::size_t detect_fields_greey(multi_range)
		{
			constexpr std::size_t middle = Begin + (End - Begin) / 2;

			return detail::template detect_fields_greey<T, Begin, middle>(is_one_element_range<Begin, middle>{});
		}

		template<detail::tuple _Ty,std::size_t N>
		constexpr std::size_t detect_fields_count_dispatch(size_t_<N>)
		{
			return std::tuple_size_v<_Ty>();
		}

		template<typename _Ty, std::size_t N>
		constexpr std::size_t detect_fields_count_dispatch(size_t_<N>)
		{
			return detail::template detect_fields_greey<_Ty, 0, N>(multi_range{});
		}

		template<typename _Ty>
		constexpr std::size_t fields_count()
		{
			using type = std::remove_cv_t<_Ty>;

			constexpr std::size_t max_fields_count = sizeof(type) * CHAR_BIT;

			constexpr std::size_t result = detail::template detect_fields_count_dispatch<type>(size_t_<max_fields_count>{});

			return result;
		}
	}
}