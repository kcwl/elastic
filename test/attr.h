#pragma once
#include <boost/test/unit_test_suite.hpp>
#include <sstream>
#include "../include/elastic/types/types.hpp"
#include "../include/elastic/binary_iarchive.hpp"
#include "../include/elastic/binary_oarchive.hpp"

BOOST_AUTO_TEST_SUITE(attr)

BOOST_AUTO_TEST_CASE(attr_to_iostream)
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
		elastic::repeated<int> value{ 1, 2, 3 };

		std::stringstream buf;
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

		std::stringstream buf;
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

		std::stringstream buf;
		elastic::binary_oarchive oa(buf);

		oa << value;

		elastic::fixed64 value1{};

		elastic::binary_iarchive ia(buf);
		ia >> value1;

		BOOST_TEST(value.value_ == value1.value_);
	}

	{
		elastic::uint32 value{};
		value = 2;

		std::stringstream buf;
		elastic::binary_oarchive oa(buf);

		oa << value;

		elastic::uint32 value1{};

		elastic::binary_iarchive ia(buf);
		ia >> value1;

		BOOST_TEST(value == value1);
	}
	{
		enum class color
		{
			red = 1,
			blue = 2
		};

		std::stringstream buf;
		elastic::binary_oarchive oa(buf);

		oa << color::red;

		color cr{};

		elastic::binary_iarchive ia(buf);
		ia >> cr;

		BOOST_CHECK(cr == color::red);
	}
}

BOOST_AUTO_TEST_SUITE_END()