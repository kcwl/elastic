#pragma once
#include <boost/test/unit_test_suite.hpp>
#include <elastic.hpp>

BOOST_AUTO_TEST_SUITE(inherit)

struct base
{
	int a;
	int b;
};

struct deri : base
{
	int c;
	int d;

private:
	friend class elastic::access;
	template <typename _Archive>
	void serialize(_Archive& ar)
	{
		ar& elastic::base_object<base>(*this);
		ar& c;
		ar& d;
	}
};

BOOST_AUTO_TEST_CASE(inherit)
{
	{
		deri d{};
		d.a = 1;
		d.b = 2;
		d.c = 3;
		d.d = 4;

		elastic::flex_buffer_t buf;

		elastic::to_binary(d, buf);

		deri dd{};

		elastic::from_binary(dd, buf);

		BOOST_CHECK(d.a == dd.a && d.b == dd.b && d.c == dd.c && d.d == dd.d);
	}

	{
		elastic::flex_buffer_t buf;
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

		elastic::to_binary(p_in, buf);

		person p_out{};

		elastic::from_binary(p_out, buf);

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

		elastic::flex_buffer_t buf;

		elastic::to_binary(1, buf);
		elastic::to_binary(2, buf);

		person p1{};

		elastic::from_binary(p1, buf);

		BOOST_CHECK_EQUAL(p1.a_, 0);
		BOOST_CHECK_EQUAL(p1.b_, 0);
		BOOST_CHECK_EQUAL(p1.s_.b_, 0);
	}
}

BOOST_AUTO_TEST_SUITE_END()
