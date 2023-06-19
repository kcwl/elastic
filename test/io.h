#pragma once
#include <boost/test/unit_test_suite.hpp>
#include "../include/elastic/serialize/streambuf.hpp"
#include "../include/elastic/archive/binary_iarchive.hpp"
#include "../include/elastic/archive/binary_oarchive.hpp"

BOOST_AUTO_TEST_SUITE(io)

BOOST_AUTO_TEST_CASE(iostream)
{
	{
		elastic::streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		char a_in = (std::numeric_limits<char>::max)();

		oa << a_in;

		char a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}

	{
		elastic::streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		int8_t a_in = (std::numeric_limits<int8_t>::max)();

		oa << a_in;

		int8_t a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}

	{
		elastic::streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		uint8_t a_in = (std::numeric_limits<uint8_t>::max)();

		oa << a_in;

		uint8_t a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}

	{
		elastic::streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		int16_t a_in = (std::numeric_limits<int16_t>::max)();

		oa << a_in;

		int16_t a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}
	{
		elastic::streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		uint16_t a_in = (std::numeric_limits<uint16_t>::max)();

		oa << a_in;

		uint16_t a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}

	{
		elastic::streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		int32_t a_in = (std::numeric_limits<int32_t>::max)();

		oa << a_in;

		int32_t a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}

	{
		elastic::streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		uint32_t a_in = (std::numeric_limits<uint32_t>::max)();

		oa << a_in;

		uint32_t a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}

	{
		elastic::streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		int64_t a_in = (std::numeric_limits<int64_t>::max)();

		oa << a_in;

		int64_t a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}

	{
		elastic::streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		uint64_t a_in = (std::numeric_limits<uint64_t>::max)();

		oa << a_in;

		uint64_t a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}

	{
		elastic::streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		std::vector<int32_t> a_in = { 1, 2, 3, 4, 5 };

		oa << a_in;

		std::vector<int32_t> a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}

	{
		elastic::streambuf<char, std::char_traits<char>> buf;
		elastic::binary_oarchive oa(buf);

		std::string a_in = "hello world!";

		oa << a_in;

		std::string a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}

	{
		elastic::streambuf<char, std::char_traits<char>> buf;
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

BOOST_AUTO_TEST_SUITE_END()