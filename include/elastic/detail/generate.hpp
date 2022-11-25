#pragma once
#include <elastic/detail/size_t_.hpp>
#include <elastic/detail/concepts.hpp>

namespace elastic
{
	namespace detail
	{
		template <std::size_t Index>
		using size_t_ = std::integral_constant<std::size_t, Index >;

		template<typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<0>) noexcept
		{
			return std::make_tuple();
		}

		template<class_cpt _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<1>) noexcept
		{
			auto& [a] = val;
			return std::make_tuple(a);
		}

		template<typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<1>) noexcept
		{
			auto& a = val;
			return std::make_tuple(a);
		}

		template<typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<2>) noexcept
		{
			auto& [a, b] = val;
			return std::make_tuple(a, b);
		}

		template<typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<3>) noexcept
		{
			auto& [a, b, c] = val;
			return std::make_tuple(a, b, c);
		}

		template<typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<4>) noexcept
		{
			auto& [a, b, c, d] = val;
			return std::make_tuple(a, b, c, d);
		}

		template<typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<5>) noexcept
		{
			auto& [a, b, c, d, e] = val;
			return std::make_tuple(a, b, c, d, e);
		}

		template<typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<6>) noexcept
		{
			auto& [a, b, c, d, e, f] = val;
			return std::make_tuple(a, b, c, d, e, f);
		}

		template<typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<7>) noexcept
		{
			auto& [a, b, c, d, e, f, g] = val;
			return std::make_tuple(a, b, c, d, e, f, g);
		}

		template<typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<8>) noexcept
		{
			auto& [a, b, c, d, e, f, g, h] = val;
			return std::make_tuple(a, b, c, d, e, f, g, h);
		}

		template<typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<9>) noexcept
		{
			auto& [a, b, c, d, e, f, g, h, i] = val;
			return std::make_tuple(a, b, c, d, e, f, g, h, i);
		}

		template<typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<10>) noexcept
		{
			auto& [a, b, c, d, e, f, g, h, i, j] = val;
			return std::make_tuple(a, b, c, d, e, f, g, h, i, j);
		}

		template<typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<11>) noexcept
		{
			auto& [a, b, c, d, e, f, g, h, i, j, k] = val;
			return std::make_tuple(a, b, c, d, e, f, g, h, i, j, k);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<12>) noexcept
		{
			auto& [a, b, c, d, e, f, g, h, j, k, l, m] = val;
			return std::make_tuple(a, b, c, d, e, f, g, h, j, k, l, m);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<13>) noexcept
		{
			auto& [a, b, c, d, e, f, g, h, j, k, l, m, n] = val;
			return std::make_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<14>) noexcept
		{
			auto& [a, b, c, d, e, f, g, h, j, k, l, m, n, p] = val;
			return std::make_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<15>) noexcept
		{
			auto& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q] = val;
			return std::make_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<16>) noexcept
		{
			auto& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r] = val;
			return std::make_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<17>) noexcept
		{
			auto& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s] = val;
			return std::make_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<18>) noexcept
		{
			auto& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t] = val;
			return std::make_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<19>) noexcept
		{
			auto& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u] = val;
			return std::make_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<20>) noexcept
		{
			auto& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v] = val;
			return std::make_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<21>) noexcept
		{
			auto& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w] = val;
			return std::make_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<22>) noexcept
		{
			auto& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x] = val;
			return std::make_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<23>) noexcept
		{
			auto& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y] = val;
			return std::make_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<24>) noexcept
		{
			auto& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z] = val;
			return std::make_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<25>) noexcept
		{
			auto& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A] = val;
			return std::make_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<26>) noexcept
		{
			auto& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A, B] = val;
			return std::make_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A, B);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<27>) noexcept
		{
			auto& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A, B, C] = val;
			return std::make_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A, B, C);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<28>) noexcept
		{
			auto& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A, B, C, D] = val;
			return std::make_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A, B, C, D);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<29>) noexcept
		{
			auto& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A, B, C, D, E] = val;
			return std::make_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A, B, C, D, E);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<30>) noexcept
		{
			auto& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A, B, C, D, E, F] = val;
			return std::make_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A, B, C, D, E, F);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<31>) noexcept
		{
			auto& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A, B, C, D, E, F, G] = val;
			return std::make_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A, B, C, D, E, F, G);
		}

		template <typename _Ty>
		constexpr auto make_tuple(_Ty& val, size_t_<32>) noexcept
		{
			auto& [a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A, B, C, D, E, F, G, H] = val;
			return std::make_tuple(a, b, c, d, e, f, g, h, j, k, l, m, n, p, q, r, s, t, u, v, w, x, y, z, A, B, C, D, E, F, G, H);
		}
	}
}