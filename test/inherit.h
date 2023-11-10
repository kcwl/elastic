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

		std::stringstream buf;
		elastic::binary_oarchive oa(buf);

		oa << d;

		deri dd{};
		elastic::binary_iarchive ia(buf);
		ia >> dd;

		BOOST_CHECK(d.a == dd.a && d.b == dd.b && d.c == dd.c && d.d == dd.d);
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
