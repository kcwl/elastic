#pragma once
#include "../include/elastic/type.hpp"
#include "../include/elastic/archive.hpp"

#include <boost/test/unit_test_suite.hpp>
#include <sstream>

BOOST_AUTO_TEST_SUITE(prop)

BOOST_AUTO_TEST_CASE(elastic_property)
{
	{
		elastic::optional<int> value{ 1 };

		std::stringstream buf;
		elastic::binary_oarchive oa(buf);

		oa << value;

		elastic::optional<int> value1;

		elastic::binary_iarchive ia(buf);
		ia >> value1;

		BOOST_TEST(value.value() == value1.value());
	}
	{
		elastic::fixed<uint32_t> value{};
		value = 2;

		std::stringstream buf;
		elastic::binary_oarchive oa(buf);

		oa << value;

		elastic::fixed<uint32_t> value1{};

		elastic::binary_iarchive ia(buf);
		ia >> value1;

		BOOST_TEST(value.value_ == value1.value_);
	}

	{
		elastic::fixed<uint64_t> value{};
		value = 2;

		std::stringstream buf;
		elastic::binary_oarchive oa(buf);

		oa << value;

		elastic::fixed<uint64_t> value1{};

		elastic::binary_iarchive ia(buf);
		ia >> value1;

		BOOST_CHECK_EQUAL(value.value_, value1.value_);
	}
	{
		std::stringstream buf;
		elastic::binary_oarchive oa(buf);

		std::vector<int32_t> a_in = { 1, 2, 3, 4, 5 };

		oa << a_in;

		std::vector<int32_t> a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}
}

BOOST_AUTO_TEST_SUITE_END()