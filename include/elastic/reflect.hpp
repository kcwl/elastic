#pragma once
#include <string_view>
#include "concepts.hpp"

using namespace std::string_view_literals;

namespace elastic
{
	template <std::size_t N>
	using size_t_ = std::integral_constant<std::size_t, N>;

	namespace detail
	{
		using one_range = std::true_type;
		using multi_range = std::false_type;

		template <std::size_t Begin, std::size_t End>
		using is_one_element_range = std::bool_constant<Begin == End>;

		template <typename _Ty, std::size_t Start, std::size_t End>
		constexpr std::size_t detect_fields_greey(one_range)
		{
			static_assert(Start == End, "It is not a correct way with binary search!");

			if constexpr (aggregate_inialize<_Ty, Start>)
			{
				return Start;
			}
			else
			{
				return 0;
			}
		}

		template <typename _Ty, std::size_t Begin, std::size_t End>
		constexpr std::size_t detect_fields_greey(multi_range);

		template <typename _Ty, std::size_t Begin, std::size_t End>
		requires(aggregate_inialize<_Ty, End>)
		constexpr auto detect_fields_greey(multi_range)
		{
			constexpr std::size_t next = End + (End - Begin + 1) / 2;

			return detail::template detect_fields_greey<_Ty, End, next>(is_one_element_range<End, next>{});
		}

		template <typename _Ty, std::size_t Begin, std::size_t End>
		constexpr std::size_t detect_fields_greey(multi_range)
		{
			constexpr std::size_t middle = Begin + (End - Begin) / 2;

			return detail::template detect_fields_greey<_Ty, Begin, middle>(is_one_element_range<Begin, middle>{});
		}

		template <typename _Ty, std::size_t N>
		constexpr std::size_t detect_fields_count_dispatch(size_t_<N>)
		{
			return detail::template detect_fields_greey<_Ty, 0, N>(multi_range{});
		}

		template <tuple_t _Ty>
		constexpr std::size_t fields_count()
		{
			using type = std::remove_cv_t<_Ty>;

			return std::tuple_size_v<type>;
		}

		template <typename _Ty>
		constexpr std::size_t fields_count()
		{
			using type = std::remove_cv_t<_Ty>;

			constexpr std::size_t max_fields_count = sizeof(type) * CHAR_BIT;

			constexpr std::size_t result =
				detail::template detect_fields_count_dispatch<type>(size_t_<max_fields_count>{});

			return result;
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&&, size_t_<0>) noexcept
		{
			return std::forward_as_tuple();
		}

		template <class_cpt _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<1>) noexcept
		{
			auto&& [a] = val;
			return std::forward_as_tuple(a);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<1>) noexcept
		{
			auto&& a = val;
			return std::forward_as_tuple(a);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<2>) noexcept
		{
			auto&& [a, b] = val;
			return std::forward_as_tuple(a, b);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<3>) noexcept
		{
			auto&& [a, b, c] = val;
			return std::forward_as_tuple(a, b, c);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<4>) noexcept
		{
			auto&& [a, b, c, d] = std::forward<_Ty>(val);
			return std::forward_as_tuple(a, b, c, d);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<5>) noexcept
		{
			auto&& [a, b, c, d, e] = val;
			return std::forward_as_tuple(a, b, c, d, e);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<6>) noexcept
		{
			auto&& [a, b, c, d, e, f] = val;
			return std::forward_as_tuple(a, b, c, d, e, f);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<7>) noexcept
		{
			auto&& [a, b, c, d, e, f, g] = val;
			return std::forward_as_tuple(a, b, c, d, e, f, g);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<8>) noexcept
		{
			auto&& [a, b, c, d, e, f, g, h] = val;
			return std::forward_as_tuple(a, b, c, d, e, f, g, h);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<9>) noexcept
		{
			auto&& [a, b, c, d, e, f, g, h, i] = val;
			return std::forward_as_tuple(a, b, c, d, e, f, g, h, i);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<10>) noexcept
		{
			auto&& [a, b, c, d, e, f, g, h, i, j] = val;
			return std::forward_as_tuple(a, b, c, d, e, f, g, h, i, j);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<11>) noexcept
		{
			auto&& [a, b, c, d, e, f, g, h, i, j, k] = val;
			return std::forward_as_tuple(a, b, c, d, e, f, g, h, i, j, k);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<12>) noexcept
		{
			auto&& [a, b, c, d, e, f, g, h, j, k, l, m] = val;
			return std::forward_as_tuple(a, b, c, d, e, f, g, h, j, k, l, m);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<13>) noexcept
		{
			auto&& [a, b, c, d, e, f, g, h, j, k, l, m, n] = val;
			return std::forward_as_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<14>) noexcept
		{
			auto&& [a, b, c, d, e, f, g, h, j, k, l, m, n, p] = val;
			return std::forward_as_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<15>) noexcept
		{
			auto&& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q] = val;
			return std::forward_as_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<16>) noexcept
		{
			auto&& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r] = val;
			return std::forward_as_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<17>) noexcept
		{
			auto&& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s] = val;
			return std::forward_as_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<18>) noexcept
		{
			auto&& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t] = val;
			return std::forward_as_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<19>) noexcept
		{
			auto&& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u] = val;
			return std::forward_as_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<20>) noexcept
		{
			auto&& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v] = val;
			return std::forward_as_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<21>) noexcept
		{
			auto&& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w] = val;
			return std::forward_as_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<22>) noexcept
		{
			auto&& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x] = val;
			return std::forward_as_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<23>) noexcept
		{
			auto&& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y] = val;
			return std::forward_as_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<24>) noexcept
		{
			auto&& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z] = val;
			return std::forward_as_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<25>) noexcept
		{
			auto&& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A] = val;
			return std::forward_as_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<26>) noexcept
		{
			auto&& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A, B] = val;
			return std::forward_as_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A, B);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<27>) noexcept
		{
			auto&& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A, B, C] = val;
			return std::forward_as_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A, B,
										 C);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<28>) noexcept
		{
			auto&& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A, B, C, D] = val;
			return std::forward_as_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A, B,
										 C, D);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<29>) noexcept
		{
			auto&& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A, B, C, D, E] = val;
			return std::forward_as_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A, B,
										 C, D, E);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<30>) noexcept
		{
			auto&& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A, B, C, D, E, F] = val;
			return std::forward_as_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A, B,
										 C, D, E, F);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<31>) noexcept
		{
			auto&& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A, B, C, D, E, F, G] = val;
			return std::forward_as_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A, B,
										 C, D, E, F, G);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty&& val, size_t_<32>) noexcept
		{
			auto&& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A, B, C, D, E, F, G, H] =
				val;
			return std::forward_as_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A, B,
										 C, D, E, F, G, H);
		}
	} // namespace detail

	template <typename _Ty>
	using tuple_size = size_t_<detail::fields_count<_Ty>()>;

	template <typename _Ty>
	constexpr std::size_t tuple_size_v = tuple_size<std::remove_cvref_t<_Ty>>::value;

	template <typename _Ty>
	constexpr auto name() -> std::string_view
	{
		using namespace std::string_view_literals;

#ifndef __linux
		constexpr std::string_view name = __FUNCSIG__ ""sv;
#else
		constexpr std::string_view name = __PRETTY_FUNCTION__;
#endif

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
		return std::get<N>(detail::template make_tuple(val, size_t_<elastic::tuple_size_v<_Ty>>{}));
	}

	template <std::size_t I, typename _Tuple>
	struct tuple_element
	{
		using type = decltype(elastic::get<I>(std::declval<_Tuple>()));
	};

	template <std::size_t I, typename _Tuple>
	using tuple_element_t = typename tuple_element<I, _Tuple>::type;

} // namespace elastic