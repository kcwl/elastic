#pragma once
#include "../include/elastic/binary_archive.hpp"

#include <boost/test/unit_test_suite.hpp>

BOOST_AUTO_TEST_SUITE(io)

BOOST_AUTO_TEST_CASE(elastic_type)
{
	{
		std::stringstream buf;
		elastic::binary_oarchive oa(buf);

		char a_in = (std::numeric_limits<char>::max)();

		oa << a_in;

		char a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}

	{
		std::stringstream buf;
		elastic::binary_oarchive oa(buf);

		int8_t a_in = (std::numeric_limits<int8_t>::max)();

		oa << a_in;

		int8_t a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}

	{
		std::stringstream buf;
		elastic::binary_oarchive oa(buf);

		uint8_t a_in = (std::numeric_limits<uint8_t>::max)();

		oa << a_in;

		uint8_t a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}

	{
		std::stringstream buf;
		elastic::binary_oarchive oa(buf);

		int16_t a_in = (std::numeric_limits<int16_t>::max)();

		oa << a_in;

		int16_t a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}
	{
		std::stringstream buf;
		elastic::binary_oarchive oa(buf);

		uint16_t a_in = (std::numeric_limits<uint16_t>::max)();

		oa << a_in;

		uint16_t a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}

	{
		std::stringstream buf;
		elastic::binary_oarchive oa(buf);

		int32_t a_in = (std::numeric_limits<int32_t>::max)();

		oa << a_in;

		int32_t a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}

	{
		std::stringstream buf;
		elastic::binary_oarchive oa(buf);

		uint32_t a_in = (std::numeric_limits<uint32_t>::max)();

		oa << a_in;

		uint32_t a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}

	{
		std::stringstream buf;
		elastic::binary_oarchive oa(buf);

		int64_t a_in = (std::numeric_limits<int64_t>::max)();

		oa << a_in;

		int64_t a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}

	{
		std::stringstream buf;
		elastic::binary_oarchive oa(buf);

		uint64_t a_in = (std::numeric_limits<uint64_t>::max)();

		oa << a_in;

		uint64_t a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
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
	{
		std::stringstream buf;
		elastic::binary_oarchive oa(buf);

		std::string a_in = "hello world!";

		oa << a_in;

		std::string a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}
	{
		std::stringstream buf;
		elastic::binary_oarchive oa(buf);

		bool a_in = false;

		oa << a_in;

		bool a_out = true;

		elastic::binary_iarchive ia(buf);

		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}
	{
		std::stringstream buf;
		elastic::binary_oarchive oa(buf);

		std::vector<std::byte> a_in = { std::byte('1'), std::byte('2'), std::byte('3'), std::byte('4'),
										std::byte('5') };

		oa << a_in;

		std::vector<std::byte> a_out{};

		elastic::binary_iarchive ia(buf);
		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}
	{
		std::stringstream buf;
		elastic::binary_oarchive oa(buf);

		double a_in = 1.2;

		oa << a_in;

		double a_out;

		elastic::binary_iarchive ia(buf);

		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}
	{
		std::stringstream buf;
		elastic::binary_oarchive oa(buf);

		float a_in = 2.4f;

		oa << a_in;

		float a_out;

		elastic::binary_iarchive ia(buf);

		ia >> a_out;

		BOOST_TEST(a_in == a_out);
	}
	
	{
		std::stringstream buf;
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

	{
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

		std::stringstream buf;

		elastic::binary_oarchive oa(buf);

		person p{};
		p.a_ = 1;
		p.b_ = 2;

		oa << p.a_;
		oa << p.b_;

		elastic::binary_iarchive ia(buf);

		person p1{};

		ia >> p1;

		BOOST_CHECK_EQUAL(p1.a_, 0);
		BOOST_CHECK_EQUAL(p1.b_, 0);
		BOOST_CHECK_EQUAL(p1.s_.b_, 0);
	}
}

BOOST_AUTO_TEST_SUITE_END()