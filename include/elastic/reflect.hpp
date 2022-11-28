#pragma once
#include <string_view>
#include <elastic/tuple_size.hpp>
#include <elastic/detail/generate.hpp>

namespace elastic
{
	template<typename _Ty>
	constexpr auto struct_name() -> std::string_view
	{
		using namespace std::string_view_literals;

		constexpr std::string_view name = __FUNCSIG__""sv;

		return name.substr(102, name.size() - 109);
	}

	template<std::size_t N, typename _Ty>
	constexpr auto get(const _Ty& val)
	{
		return std::get<N>(detail::template make_tuple(val, detail::template size_t_<tuple_size_v<_Ty>>{}));
	}
}