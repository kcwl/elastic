#pragma once
#include <elastic.hpp>
#include <fstream>

enum class color
{
	red,
	yellow
};

TEST(buffer, function)
{
	{
		const elastic::flex_buffer_t buffer;

		EXPECT_TRUE(buffer.active() == 512);

		EXPECT_TRUE(buffer.size() == 0);

		EXPECT_TRUE(*buffer.wdata() == 0);

		EXPECT_TRUE(*buffer.rdata() == 0);
	}

	{
		elastic::flex_buffer_t buf{};

		elastic::to_binary(1, buf);

		int a = 0;

		elastic::from_binary(a, buf);

		EXPECT_TRUE(a == 1);

		buf.ensure();
		buf.normalize();

		EXPECT_TRUE(buf.size() == 0 && buf.active() == 512);
	}

	{
		elastic::flex_buffer_t buffer{ 4096 };
		buffer.ensure();
		buffer.normalize();
	}

	{
		elastic::flex_buffer_t buffer{ 10 };
		buffer.ensure();

		EXPECT_TRUE(buffer.active() == 10 + elastic::flex_buffer_t::capacity);
	}

	{
		elastic::flex_buffer_t buffer;

		buffer.commit(5);

		buffer.consume(3);

		EXPECT_TRUE(buffer.size() == 2);
	}

	{
		elastic::flex_buffer_t buffer{ 0 };

		EXPECT_TRUE(elastic::to_binary(2, buffer));
	}

	{
		elastic::flex_buffer_t buffer;

		EXPECT_TRUE(buffer.pubseekoff(5, std::ios::beg, std::ios::in) == 5);

		EXPECT_TRUE(buffer.pubseekoff(-1, std::ios::cur, std::ios::in) == 4);

		EXPECT_TRUE(buffer.pubseekoff(1, 3, std::ios::in) == -1);

		buffer.pubseekoff(3, std::ios::beg, std::ios::out);

		buffer.pubseekoff(1, std::ios::cur, std::ios::out);

		EXPECT_TRUE(buffer.size() == 0);

		EXPECT_TRUE(buffer.pubseekoff(1, std::ios::end, std::ios::in) == -1);
	}

	{
		elastic::flex_buffer_t buffer{ 3 };

		EXPECT_TRUE(buffer.pubseekpos(5, std::ios::in) == 2);

		EXPECT_TRUE(buffer.pubseekpos(2, std::ios::in) = 2);
	}

	{
		elastic::flex_buffer_t buffer{};
		EXPECT_TRUE(buffer.pubseekpos(1, 0) == -1);
	}

	{
		elastic::flex_buffer_t buffer{ 0 };

		EXPECT_TRUE(buffer.pubseekoff(1, std::ios::cur, std::ios::out) == -1);
	}

	{
		elastic::flex_buffer_t buffer{};

		buffer.commit(5);

		EXPECT_TRUE(buffer.pubseekpos(10, std::ios::out) == -1);
	}

	{
		elastic::flex_buffer_t buffer{ 0 };

		bool a{};

		EXPECT_FALSE(elastic::from_binary(a, buffer));

		int b{};

		EXPECT_FALSE(elastic::from_binary(b, buffer));

		std::string c{};

		EXPECT_FALSE(elastic::from_binary(c, buffer));

		std::vector<char> d{};

		EXPECT_FALSE(elastic::from_binary(d, buffer));

		color e{};

		EXPECT_FALSE(elastic::from_binary(e, buffer));

		float f{};

		EXPECT_FALSE(elastic::from_binary(f, buffer));

		int temp = 1;

		buffer.save((uint8_t*)&temp, 1);

		EXPECT_FALSE(elastic::from_binary(c, buffer));

		buffer.save((uint8_t*)&temp, 1);

		EXPECT_FALSE(elastic::from_binary(c, buffer));

		EXPECT_FALSE(elastic::from_binary(d, buffer));
	}
}