#pragma once
#include <elastic.hpp>
#include <optional>
#include <boost/test/unit_test_suite.hpp>

BOOST_AUTO_TEST_SUITE(prop)

BOOST_AUTO_TEST_CASE(elastic_property)
{
	{
		std::optional<int> value{ 1 };

		elastic::flex_buffer_t buf;
		elastic::binary_oarchive oa(buf);

		oa << value;

		std::optional<int> value1;

		elastic::binary_iarchive ia(buf);
		ia >> value1;

		BOOST_TEST(value.value() == value1.value());
	}

	{
		elastic::flex_buffer_t buf;
		elastic::binary_oarchive oa(buf);

		std::vector<std::byte> a_in = { std::byte(1), std::byte(2), std::byte(3), std::byte(4), std::byte(5) };

		oa << a_in;

		std::vector<std::byte> a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}
}

BOOST_AUTO_TEST_SUITE_END()