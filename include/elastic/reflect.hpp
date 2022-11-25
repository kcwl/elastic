#pragma once
#include <string_view>
#include <elastic/tuple_size.hpp>

namespace elastic
{
	namespace detail
	{
		template<typename _Ty>
		constexpr auto struct_name() -> std::string_view
		{
			constexpr auto f = []<template _Ty>()
			{
				using namespace std::string_view_literals;

				constexpr std::string_view name = __FUNCSIG__""sv;

				return name.substr(114, name.size() - 121);
			};

			return f<_Ty>();
		}

		template<std::size_t N, typename _Ty>
		constexpr auto get(const _Ty& val)
		{
			return std::get<N>(detail::template make_tuple(val, size_t_<detail::tuple_size_v<_Ty>>{}));
		}
	}
}