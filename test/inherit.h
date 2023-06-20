#pragma once
#include <boost/test/unit_test_suite.hpp>
#include "../include/elastic/access.hpp"
#include "../include/elastic/binary_iarchive.hpp"
#include "../include/elastic/binary_oarchive.hpp"

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

BOOST_AUTO_TEST_SUITE_END()

