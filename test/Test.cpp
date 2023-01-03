#define BOOST_TEST_MODULE Test
#include <array>
#include <boost/test/included/unit_test.hpp>
#include <elastic.hpp>
#include <limits>
#include <tuple>

struct person
{
	int a;
	char b;
};

BOOST_AUTO_TEST_CASE(tuplesize)
{
	static_assert(elastic::tuple_size_v<person> == 2, "size error!");

	static_assert(elastic::tuple_size_v<std::array<int, 4>> == 4, "size error!");

	static_assert(elastic::tuple_size_v<std::tuple<int, char>> == 2, "size error!");
}

struct test
{};

struct test1
{
	int a_;
};

struct test2
{
	int a_;
	int b_;
};

struct test3
{
	int a_;
	int b_;
	int c_;
};

struct test4
{
	int a_;
	int b_;
	int c_;
	int d_;
};

struct test5
{
	int a_;
	int b_;
	int c_;
	int d_;
	int e_;
};

struct test6
{
	int a_;
	int b_;
	int c_;
	int d_;
	int e_;
	int f_;
};

struct test7
{
	int a_;
	int b_;
	int c_;
	int d_;
	int e_;
	int f_;
	int g_;
};

struct test8
{
	int a_;
	int b_;
	int c_;
	int d_;
	int e_;
	int f_;
	int g_;
	int h_;
};

struct test9
{
	int a_;
	int b_;
	int c_;
	int d_;
	int e_;
	int f_;
	int g_;
	int h_;
	int i_;
};

struct test10
{
	int a_;
	int b_;
	int c_;
	int d_;
	int e_;
	int f_;
	int g_;
	int h_;
	int i_;
	int j_;
};

struct test11
{
	int a_;
	int b_;
	int c_;
	int d_;
	int e_;
	int f_;
	int g_;
	int h_;
	int i_;
	int j_;
	int k_;
};

struct test12
{
	int a_;
	int b_;
	int c_;
	int d_;
	int e_;
	int f_;
	int g_;
	int h_;
	int i_;
	int j_;
	int k_;
	int l_;
};

struct test13
{
	int a_;
	int b_;
	int c_;
	int d_;
	int e_;
	int f_;
	int g_;
	int h_;
	int i_;
	int j_;
	int k_;
	int l_;
	int m_;
};

struct test14
{
	int a_;
	int b_;
	int c_;
	int d_;
	int e_;
	int f_;
	int g_;
	int h_;
	int i_;
	int j_;
	int k_;
	int l_;
	int m_;
	int n_;
};

struct test15
{
	int a_;
	int b_;
	int c_;
	int d_;
	int e_;
	int f_;
	int g_;
	int h_;
	int i_;
	int j_;
	int k_;
	int l_;
	int m_;
	int n_;
	int o_;
};

struct test16
{
	int a_;
	int b_;
	int c_;
	int d_;
	int e_;
	int f_;
	int g_;
	int h_;
	int i_;
	int j_;
	int k_;
	int l_;
	int m_;
	int n_;
	int o_;
	int p_;
};

struct test17
{
	int a_;
	int b_;
	int c_;
	int d_;
	int e_;
	int f_;
	int g_;
	int h_;
	int i_;
	int j_;
	int k_;
	int l_;
	int m_;
	int n_;
	int o_;
	int p_;
	int q_;
};

struct test18
{
	int a_;
	int b_;
	int c_;
	int d_;
	int e_;
	int f_;
	int g_;
	int h_;
	int i_;
	int j_;
	int k_;
	int l_;
	int m_;
	int n_;
	int o_;
	int p_;
	int q_;
	int r_;
};

struct test19
{
	int a_;
	int b_;
	int c_;
	int d_;
	int e_;
	int f_;
	int g_;
	int h_;
	int i_;
	int j_;
	int k_;
	int l_;
	int m_;
	int n_;
	int o_;
	int p_;
	int q_;
	int r_;
	int s_;
};

struct test20
{
	int a_;
	int b_;
	int c_;
	int d_;
	int e_;
	int f_;
	int g_;
	int h_;
	int i_;
	int j_;
	int k_;
	int l_;
	int m_;
	int n_;
	int o_;
	int p_;
	int q_;
	int r_;
	int s_;
	int t_;
};

struct test21
{
	int a_;
	int b_;
	int c_;
	int d_;
	int e_;
	int f_;
	int g_;
	int h_;
	int i_;
	int j_;
	int k_;
	int l_;
	int m_;
	int n_;
	int o_;
	int p_;
	int q_;
	int r_;
	int s_;
	int t_;
	int u_;
};

struct test22
{
	int a_;
	int b_;
	int c_;
	int d_;
	int e_;
	int f_;
	int g_;
	int h_;
	int i_;
	int j_;
	int k_;
	int l_;
	int m_;
	int n_;
	int o_;
	int p_;
	int q_;
	int r_;
	int s_;
	int t_;
	int u_;
	int v_;
};

struct test23
{
	int a_;
	int b_;
	int c_;
	int d_;
	int e_;
	int f_;
	int g_;
	int h_;
	int i_;
	int j_;
	int k_;
	int l_;
	int m_;
	int n_;
	int o_;
	int p_;
	int q_;
	int r_;
	int s_;
	int t_;
	int u_;
	int v_;
	int w_;
};

struct test24
{
	int a_;
	int b_;
	int c_;
	int d_;
	int e_;
	int f_;
	int g_;
	int h_;
	int i_;
	int j_;
	int k_;
	int l_;
	int m_;
	int n_;
	int o_;
	int p_;
	int q_;
	int r_;
	int s_;
	int t_;
	int u_;
	int v_;
	int w_;
	int x_;
};

struct test25
{
	int a_;
	int b_;
	int c_;
	int d_;
	int e_;
	int f_;
	int g_;
	int h_;
	int i_;
	int j_;
	int k_;
	int l_;
	int m_;
	int n_;
	int o_;
	int p_;
	int q_;
	int r_;
	int s_;
	int t_;
	int u_;
	int v_;
	int w_;
	int x_;
	int y_;
};

struct test26
{
	int a_;
	int b_;
	int c_;
	int d_;
	int e_;
	int f_;
	int g_;
	int h_;
	int i_;
	int j_;
	int k_;
	int l_;
	int m_;
	int n_;
	int o_;
	int p_;
	int q_;
	int r_;
	int s_;
	int t_;
	int u_;
	int v_;
	int w_;
	int x_;
	int y_;
	int z_;
};

struct test27
{
	int a_;
	int b_;
	int c_;
	int d_;
	int e_;
	int f_;
	int g_;
	int h_;
	int i_;
	int j_;
	int k_;
	int l_;
	int m_;
	int n_;
	int o_;
	int p_;
	int q_;
	int r_;
	int s_;
	int t_;
	int u_;
	int v_;
	int w_;
	int x_;
	int y_;
	int z_;
	int aA_;
};

struct test28
{
	int a_;
	int b_;
	int c_;
	int d_;
	int e_;
	int f_;
	int g_;
	int h_;
	int i_;
	int j_;
	int k_;
	int l_;
	int m_;
	int n_;
	int o_;
	int p_;
	int q_;
	int r_;
	int s_;
	int t_;
	int u_;
	int v_;
	int w_;
	int x_;
	int y_;
	int z_;
	int aA_;
	int aB_;
};

struct test29
{
	int a_;
	int b_;
	int c_;
	int d_;
	int e_;
	int f_;
	int g_;
	int h_;
	int i_;
	int j_;
	int k_;
	int l_;
	int m_;
	int n_;
	int o_;
	int p_;
	int q_;
	int r_;
	int s_;
	int t_;
	int u_;
	int v_;
	int w_;
	int x_;
	int y_;
	int z_;
	int aA_;
	int aB_;
	int aC_;
};

struct test30
{
	int a_;
	int b_;
	int c_;
	int d_;
	int e_;
	int f_;
	int g_;
	int h_;
	int i_;
	int j_;
	int k_;
	int l_;
	int m_;
	int n_;
	int o_;
	int p_;
	int q_;
	int r_;
	int s_;
	int t_;
	int u_;
	int v_;
	int w_;
	int x_;
	int y_;
	int z_;
	int aA_;
	int aB_;
	int aC_;
	int aD_;
};

struct test31
{
	int a_;
	int b_;
	int c_;
	int d_;
	int e_;
	int f_;
	int g_;
	int h_;
	int i_;
	int j_;
	int k_;
	int l_;
	int m_;
	int n_;
	int o_;
	int p_;
	int q_;
	int r_;
	int s_;
	int t_;
	int u_;
	int v_;
	int w_;
	int x_;
	int y_;
	int z_;
	int aA_;
	int aB_;
	int aC_;
	int aD_;
	int aE_;
};

struct test32
{
	int a_;
	int b_;
	int c_;
	int d_;
	int e_;
	int f_;
	int g_;
	int h_;
	int i_;
	int j_;
	int k_;
	int l_;
	int m_;
	int n_;
	int o_;
	int p_;
	int q_;
	int r_;
	int s_;
	int t_;
	int u_;
	int v_;
	int w_;
	int x_;
	int y_;
	int z_;
	int aA_;
	int aB_;
	int aC_;
	int aD_;
	int aE_;
	int aF_;
};

BOOST_AUTO_TEST_CASE(generate)
{
	test t{};
	static_assert(elastic::detail::make_tuple(t, elastic::detail::size_t_<0>{}) == std::tuple<>());

	constexpr test1 t1{ 1 };
	static_assert(elastic::detail::make_tuple(t1, elastic::detail::size_t_<1>{}) == std::tuple<int>(1));

	constexpr test2 t2{ 1, 1 };
	static_assert(elastic::detail::make_tuple(t2, elastic::detail::size_t_<2>{}) == std::tuple<int, int>(1, 1));

	constexpr test3 t3{ 1, 1, 1 };
	static_assert(elastic::detail::make_tuple(t3, elastic::detail::size_t_<3>{}) == std::tuple<int, int, int>(1, 1, 1));

	constexpr test4 t4{ 1, 1, 1, 1 };
	static_assert(elastic::detail::make_tuple(t4, elastic::detail::size_t_<4>{}) ==
				  std::tuple<int, int, int, int>(1, 1, 1, 1));

	constexpr test5 t5{ 1, 1, 1, 1, 1 };
	static_assert(elastic::detail::make_tuple(t5, elastic::detail::size_t_<5>{}) ==
				  std::tuple<int, int, int, int, int>(1, 1, 1, 1, 1));

	constexpr test6 t6{ 1, 1, 1, 1, 1, 1 };
	static_assert(elastic::detail::make_tuple(t6, elastic::detail::size_t_<6>{}) ==
				  std::tuple<int, int, int, int, int, int>(1, 1, 1, 1, 1, 1));

	constexpr test7 t7{ 1, 1, 1, 1, 1, 1, 1 };
	static_assert(elastic::detail::make_tuple(t7, elastic::detail::size_t_<7>{}) ==
				  std::tuple<int, int, int, int, int, int, int>(1, 1, 1, 1, 1, 1, 1));

	constexpr test8 t8{ 1, 1, 1, 1, 1, 1, 1, 1 };
	static_assert(elastic::detail::make_tuple(t8, elastic::detail::size_t_<8>{}) ==
				  std::tuple<int, int, int, int, int, int, int, int>(1, 1, 1, 1, 1, 1, 1, 1));

	constexpr test9 t9{ 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	static_assert(elastic::detail::make_tuple(t9, elastic::detail::size_t_<9>{}) ==
				  std::tuple<int, int, int, int, int, int, int, int, int>(1, 1, 1, 1, 1, 1, 1, 1, 1));

	constexpr test10 t10{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	static_assert(elastic::detail::make_tuple(t10, elastic::detail::size_t_<10>{}) ==
				  std::tuple<int, int, int, int, int, int, int, int, int, int>(1, 1, 1, 1, 1, 1, 1, 1, 1, 1));

	constexpr test11 t11{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	static_assert(elastic::detail::make_tuple(t11, elastic::detail::size_t_<11>{}) ==
				  std::tuple<int, int, int, int, int, int, int, int, int, int, int>(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1));

	constexpr test12 t12{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	static_assert(
		elastic::detail::make_tuple(t12, elastic::detail::size_t_<12>{}) ==
		std::tuple<int, int, int, int, int, int, int, int, int, int, int, int>(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1));

	constexpr test13 t13{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	static_assert(elastic::detail::make_tuple(t13, elastic::detail::size_t_<13>{}) ==
				  std::tuple<int, int, int, int, int, int, int, int, int, int, int, int, int>(1, 1, 1, 1, 1, 1, 1, 1, 1,
																							  1, 1, 1, 1));

	constexpr test14 t14{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	static_assert(elastic::detail::make_tuple(t14, elastic::detail::size_t_<14>{}) ==
				  std::tuple<int, int, int, int, int, int, int, int, int, int, int, int, int, int>(
					  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1));

	constexpr test15 t15{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	static_assert(elastic::detail::make_tuple(t15, elastic::detail::size_t_<15>{}) ==
				  std::tuple<int, int, int, int, int, int, int, int, int, int, int, int, int, int, int>(
					  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1));

	constexpr test16 t16{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	static_assert(elastic::detail::make_tuple(t16, elastic::detail::size_t_<16>{}) ==
				  std::tuple<int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int>(
					  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1));

	constexpr test17 t17{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	static_assert(elastic::detail::make_tuple(t17, elastic::detail::size_t_<17>{}) ==
				  std::tuple<int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int>(
					  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1));

	constexpr test18 t18{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	static_assert(elastic::detail::make_tuple(t18, elastic::detail::size_t_<18>{}) ==
				  std::tuple<int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int>(
					  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1));

	constexpr test19 t19{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	static_assert(
		elastic::detail::make_tuple(t19, elastic::detail::size_t_<19>{}) ==
		std::tuple<int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int>(
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1));

	constexpr test20 t20{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	static_assert(
		elastic::detail::make_tuple(t20, elastic::detail::size_t_<20>{}) ==
		std::tuple<int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int>(
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1));

	constexpr test21 t21{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	static_assert(elastic::detail::make_tuple(t21, elastic::detail::size_t_<21>{}) ==
				  std::tuple<int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int,
							 int, int, int>(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1));

	constexpr test22 t22{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	static_assert(elastic::detail::make_tuple(t22, elastic::detail::size_t_<22>{}) ==
				  std::tuple<int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int,
							 int, int, int, int>(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1));

	constexpr test23 t23{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	static_assert(
		elastic::detail::make_tuple(t23, elastic::detail::size_t_<23>{}) ==
		std::tuple<int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int,
				   int, int, int>(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1));

	constexpr test24 t24{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	static_assert(
		elastic::detail::make_tuple(t24, elastic::detail::size_t_<24>{}) ==
		std::tuple<int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int,
				   int, int, int, int>(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1));

	constexpr test25 t25{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	static_assert(
		elastic::detail::make_tuple(t25, elastic::detail::size_t_<25>{}) ==
		std::tuple<int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int,
				   int, int, int, int, int>(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1));

	constexpr test26 t26{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	static_assert(elastic::detail::make_tuple(t26, elastic::detail::size_t_<26>{}) ==
				  std::tuple<int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int,
							 int, int, int, int, int, int, int, int>(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
																	 1, 1, 1, 1, 1, 1, 1, 1, 1));

	constexpr test27 t27{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	static_assert(elastic::detail::make_tuple(t27, elastic::detail::size_t_<27>{}) ==
				  std::tuple<int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int,
							 int, int, int, int, int, int, int, int, int>(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
																		  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1));

	constexpr test28 t28{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	static_assert(elastic::detail::make_tuple(t28, elastic::detail::size_t_<28>{}) ==
				  std::tuple<int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int,
							 int, int, int, int, int, int, int, int, int, int>(
					  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1));

	constexpr test29 t29{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	static_assert(elastic::detail::make_tuple(t29, elastic::detail::size_t_<29>{}) ==
				  std::tuple<int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int,
							 int, int, int, int, int, int, int, int, int, int, int>(
					  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1));

	constexpr test30 t30{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	static_assert(elastic::detail::make_tuple(t30, elastic::detail::size_t_<30>{}) ==
				  std::tuple<int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int,
							 int, int, int, int, int, int, int, int, int, int, int, int>(
					  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1));

	constexpr test31 t31{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	static_assert(elastic::detail::make_tuple(t31, elastic::detail::size_t_<31>{}) ==
				  std::tuple<int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int,
							 int, int, int, int, int, int, int, int, int, int, int, int, int>(
					  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1));

	constexpr test32 t32{
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
	};
	static_assert(elastic::detail::make_tuple(t32, elastic::detail::size_t_<32>{}) ==
				  std::tuple<int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int,
							 int, int, int, int, int, int, int, int, int, int, int, int, int, int>(
					  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1));
}

BOOST_AUTO_TEST_CASE(reflect)
{
	constexpr test3 t3{ 1, 2, 3 };

	static_assert(elastic::get<0>(t3) == 1, "get value error!");
	static_assert(elastic::get<1>(t3) == 2, "get value error!");
	static_assert(elastic::get<2>(t3) == 3, "get value error!");
	static_assert(elastic::struct_name<test3>() == "test3", "get name error!");
}

BOOST_AUTO_TEST_CASE(iostream)
{
	{
		elastic::serialize_streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		char a_in = (std::numeric_limits<char>::max)();

		oa << a_in;

		char a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}

	{
		elastic::serialize_streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		int8_t a_in = (std::numeric_limits<int8_t>::max)();

		oa << a_in;

		int8_t a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}

	{
		elastic::serialize_streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		uint8_t a_in = (std::numeric_limits<uint8_t>::max)();

		oa << a_in;

		uint8_t a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}

	{
		elastic::serialize_streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		int16_t a_in = (std::numeric_limits<int16_t>::max)();

		oa << a_in;

		int16_t a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}
	{
		elastic::serialize_streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		uint16_t a_in = (std::numeric_limits<uint16_t>::max)();

		oa << a_in;

		uint16_t a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}

	{
		elastic::serialize_streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		int32_t a_in = (std::numeric_limits<int32_t>::max)();

		oa << a_in;

		int32_t a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}

	{
		elastic::serialize_streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		uint32_t a_in = (std::numeric_limits<uint32_t>::max)();

		oa << a_in;

		uint32_t a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}

	{
		elastic::serialize_streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		int64_t a_in = (std::numeric_limits<int64_t>::max)();

		oa << a_in;

		int64_t a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}

	{
		elastic::serialize_streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		uint64_t a_in = (std::numeric_limits<uint64_t>::max)();

		oa << a_in;

		uint64_t a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}

	{
		elastic::serialize_streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		std::vector<int32_t> a_in = { 1, 2, 3, 4, 5 };

		oa << a_in;

		std::vector<int32_t> a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}

	{
		elastic::serialize_streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		std::string a_in = "hello world!";

		oa << a_in;

		std::string a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}

	{
		elastic::serialize_streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		struct son
		{
			int b_;
		};

		struct person
		{
			int a_;
			int b_;
			son s_;
		};

		person p_in{ 1, 2, { 3 } };

		oa << p_in;

		elastic::binary_iarchive ia(buf);
		person p_out{};
		ia >> p_out;

		BOOST_TEST(p_in.a_ == p_out.a_);
		BOOST_TEST(p_in.b_ == p_out.b_);
		BOOST_TEST(p_in.s_.b_ == p_out.s_.b_);
	}
}

BOOST_AUTO_TEST_CASE(attr_to_iostream)
{
	{
		elastic::optional<int> value{ 1 };

		elastic::serialize_streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		oa << value;

		elastic::optional<int> value1;

		elastic::binary_iarchive ia(buf);
		ia >> value1;

		BOOST_TEST(value.value() == value1.value());
	}

	{
		elastic::require<int> value{};
		value.emplace(2);

		elastic::serialize_streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		oa << value;

		elastic::require<int> value1{};

		elastic::binary_iarchive ia(buf);
		ia >> value1;

		BOOST_TEST(value.value() == value1.value());
	}
	{
		elastic::require<int> value{};
		elastic::serialize_streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		BOOST_CHECK_THROW(oa << value, std::runtime_error);
	}

	{
		elastic::repeated<int> value{ 1, 2, 3 };

		elastic::serialize_streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		oa << value;

		elastic::repeated<int> value1{};

		elastic::binary_iarchive ia(buf);
		ia >> value1;

		BOOST_TEST(value == value1);
	}

	{
		elastic::fixed32 value{};
		value = 2;

		elastic::serialize_streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		oa << value;

		elastic::fixed32 value1{};

		elastic::binary_iarchive ia(buf);
		ia >> value1;

		BOOST_TEST(value.value_ == value1.value_);
	}

	{
		elastic::fixed64 value{};
		value = 2;

		elastic::serialize_streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		oa << value;

		elastic::fixed64 value1{};

		elastic::binary_iarchive ia(buf);
		ia >> value1;

		BOOST_TEST(value.value_ == value1.value_);
	}

	{
		elastic::unsign<int> value{};
		value = 2;

		elastic::serialize_streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		oa << value;

		elastic::unsign<int> value1{};

		elastic::binary_iarchive ia(buf);
		ia >> value1;

		BOOST_TEST(value.value_ == value1.value_);
	}
	{
		enum class color
		{
			red,
			blue
		};

		elastic::serialize_streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		oa << color::red;

		color cr{};

		elastic::binary_iarchive ia(buf);
		ia >> cr;

		BOOST_CHECK(cr == color::red);
	}
}

BOOST_AUTO_TEST_CASE(int128)
{
	{
		elastic::int128_t a = 4;

		BOOST_CHECK_THROW(a / 0, std::runtime_error);
	}
	{
		elastic::int128_t a = 4;

		BOOST_CHECK((a << 0) == a);

		BOOST_CHECK((a >> 0) == a);
	}
	{
		elastic::int128_t a(2, 0);
		BOOST_CHECK((a >> 64) == 2);
	}
	{
		elastic::int128_t a(0, 2);
		elastic::int128_t b(2, 0);

		BOOST_CHECK((a << 64) == b);
	}
	{
		elastic::int128_t a(2, 0);
		elastic::int128_t b(0, 2);

		BOOST_CHECK(b < a);
	}

	{
		elastic::int128_t a(0, 6);
		BOOST_CHECK((a / 2) == 3);
	}
}

struct son
{
	int a;

private:
	friend class elastic::access;

	template <typename _Archive>
	void serialize(_Archive& ar)
	{
		ar& a;
	}
};

struct grand_son : son
{
	int b;

private:
	friend class elastic::access;

	template <typename _Archive>
	void serialize(_Archive& ar)
	{
		ar& elastic::serialize::base_object<son>(*this);
		ar& b;
	}
};

BOOST_AUTO_TEST_CASE(nested)
{
	elastic::serialize_streambuf<char, std::char_traits<char>> buf;
	elastic::binary_oarchive oa(buf);

	grand_son ss{};
	ss.a = 1;
	ss.b = 2;

	oa << ss;

	grand_son sr{};
	elastic::binary_iarchive ia(buf);
	ia >> sr;

	BOOST_CHECK(ss.a == sr.a);
	BOOST_CHECK(ss.b == sr.b);
}

struct part
{
	int a;
	int b;
	int c;
};

BOOST_AUTO_TEST_CASE(serialize_buffer)
{
	{
		elastic::serialize_streambuf<char, std::char_traits<char>> buf;
		elastic::binary_iarchive ia(buf);

		int a = 1;
		ia >> a;

		BOOST_TEST(a == 1);
	}
	{
		elastic::serialize_streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		int a = 0;
		oa << 1;
		oa << 2;

		elastic::binary_iarchive ia(buf);

		ia >> a;

		part p{};

		ia >> p;

		part p1{};

		BOOST_CHECK(p.a == p1.a && p.b == p1.b && p.c == p1.c);
	}
}