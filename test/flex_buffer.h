#pragma once
#include <elastic.hpp>
#include <fstream>

TEST(buffer, function)
{
	{
		const elastic::flex_buffer_t buffer;

		EXPECT_TRUE(buffer.active() == 512);

		EXPECT_TRUE(buffer.size() == 0);

		EXPECT_TRUE(*buffer.wdata() == 0);

		EXPECT_TRUE(*buffer.rdata() == 0);
	}

	//{
	//	elastic::flex_buffer_t buf{};

	//	elastic::to_binary(1, buf);

	//	int a = 0;

	//	elastic::from_binary(a, buf);

	//	EXPECT_TRUE(a == 1);

	//	buf.ensure();
	//	buf.normalize();

	//	EXPECT_TRUE(buf.size() == 0 && buf.active() == 4098);
	//}

	//{
	//	elastic::flex_buffer_t buffer{4096};
	//	buffer.commit(4090);

	//	buffer.ensure();

	//	EXPECT_TRUE(buffer.max_size() == 4096);
	//}

	//{
	//	elastic::flex_buffer_t buffer{4096};
	//	buffer.ensure();
	//	buffer.normalize();
	//}

	//{
	//	elastic::flex_buffer_t buffer{4096};

	//	buffer.commit(4096);

	//	char a = '2';

	//	EXPECT_TRUE(elastic::to_binary(a, buffer));

	//	EXPECT_TRUE(buffer.max_size() == 4098);
	//}

	{
		elastic::flex_buffer_t buffer;

		buffer.commit(5);

		buffer.consume(3);

		EXPECT_TRUE(buffer.size() == 2);
	}

	{
		elastic::flex_buffer_t buffer;

		buffer.commit(10);

		buffer.pubseekoff(5, std::ios::beg, std::ios::in);

		EXPECT_TRUE(buffer.size() == 5);

		buffer.pubseekoff(3, std::ios::beg, std::ios::out);

		buffer.pubseekoff(1, std::ios::cur, std::ios::out);

		EXPECT_TRUE(buffer.size() == 6);

		EXPECT_TRUE(buffer.pubseekoff(1, std::ios::end, std::ios::in) == -1);
	}

	{
		elastic::flex_buffer_t buffer{0};
		int a = 0;

		elastic::from_binary(a, buffer);

		EXPECT_TRUE(!buffer.success());
	}

	{
		elastic::flex_buffer_t buffer{};

		elastic::to_binary(1, buffer);

		elastic::flex_buffer_t buf{};

		elastic::to_binary(2, buf);

		buffer.append(std::move(buf));

		int a = 0;
		int b = 0;

		elastic::from_binary(a, buffer);
		elastic::from_binary(b, buffer);

		EXPECT_TRUE(a == 1 && b == 2);
	}
}