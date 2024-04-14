#pragma once
#include <elastic.hpp>

struct base
{
	int a;
	int b;

	ELASTIC_ACCESS(a, b);
};

struct deri : base
{
	int c;
	int d;

	ELASTIC_ACCESS_IF(base, c, d)
};

struct son
{
	int b_;

	ELASTIC_ACCESS(b_);
};

struct persons
{
	int a_;
	int b_;
	son s_;

	ELASTIC_ACCESS(a_, b_, s_);
};

TEST(inherit, inherits)
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

		EXPECT_TRUE(d.a == dd.a && d.b == dd.b && d.c == dd.c && d.d == dd.d);
	}

	{
		elastic::flex_buffer_t buf;
		elastic::binary_oarchive oa(buf);

		persons p_in{ 1, 2, { 3 } };

		elastic::to_binary(p_in, buf);

		persons p_out{};

		elastic::from_binary(p_out, buf);

		EXPECT_TRUE(p_in.a_ == p_out.a_);
		EXPECT_TRUE(p_in.b_ == p_out.b_);
		EXPECT_TRUE(p_in.s_.b_ == p_out.s_.b_);
	}

	{
		elastic::flex_buffer_t buf;

		elastic::to_binary(1, buf);
		elastic::to_binary(2, buf);

		persons p1{};

		elastic::from_binary(p1, buf);

		EXPECT_EQ(p1.a_, 1);
		EXPECT_EQ(p1.b_, 2);
		EXPECT_EQ(p1.s_.b_, 0);
	}
}
