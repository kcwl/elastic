#define BOOST_TEST_MODULE tuple size
#include <boost/test/included/unit_test.hpp>
#include <elastic/tuple_size.hpp>
#include <array>
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