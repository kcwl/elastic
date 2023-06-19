#pragma once
#include "detail/generate.hpp"
#include "tuple_size.hpp"

#include <string_view>

using namespace std::string_view_literals;

namespace elastic
{
	template <typename _Ty>
	constexpr auto name() -> std::string_view
	{
		using namespace std::string_view_literals;

		constexpr std::string_view name = __FUNCSIG__ ""sv;

		constexpr auto left_bracket = name.find_last_of("<");

		constexpr auto end_bracket = name.find_last_of(">");

		constexpr auto temp_name = name.substr(left_bracket + 1, end_bracket - left_bracket - 1); 

		constexpr auto start = name.find_last_of(":");

		if constexpr (temp_name.find_last_of(":") == std::string_view::npos)
		{
			return temp_name;
		}
		else
		{
			return name.substr(start + 1, end_bracket - start - 1);
		}
	}

	template <std::size_t N, typename _Ty>
	constexpr auto&& get(_Ty&& val)
	{
		return std::get<N>(detail::template make_tuple(val, detail::template size_t_<elastic::tuple_size_v<_Ty>>{}));
	}

	template <std::size_t I, typename _Tuple>
	struct tuple_element
	{
		using type = decltype(elastic::get<I>(std::declval<_Tuple>()));
	};

	template <std::size_t I, typename _Tuple>
	using tuple_element_t = typename tuple_element<I, _Tuple>::type;

} // namespace elastic