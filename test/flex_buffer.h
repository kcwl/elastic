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
		buffer.commit(4090);

		buffer.ensure();

		EXPECT_TRUE(buffer.max_size() == 4096 + 512);
	}

	{
		elastic::flex_buffer_t buffer{ 4096 };
		buffer.ensure();
		buffer.normalize();
	}

	{
		elastic::flex_buffer_t buffer;

		buffer.commit(5);

		buffer.consume(3);

		EXPECT_TRUE(buffer.size() == 2);
	}

	//{
	//	elastic::flex_buffer_t buffer;

	//	buffer.commit(10);

	//	EXPECT_TRUE(buffer.pubseekoff(5, std::ios::beg, std::ios::in) == 5);

	//	EXPECT_TRUE(buffer.pubseekoff(-1, std::ios::cur, std::ios::in) == 9);

	//	EXPECT_TRUE(buffer.pubseekoff(1, 3, std::ios::in) == -1);

	//	buffer.pubseekoff(3, std::ios::beg, std::ios::out);

	//	buffer.pubseekoff(1, std::ios::cur, std::ios::out);

	//	EXPECT_TRUE(buffer.size() == 6);

	//	EXPECT_TRUE(buffer.pubseekoff(1, std::ios::end, std::ios::in) == -1);
	//}

	//{
	//	elastic::flex_buffer_t buffer{3};

	//	EXPECT_TRUE(buffer.pubseekpos(5, std::ios::in) == -1);

	//	EXPECT_TRUE(buffer.pubseekpos(2, std::ios::in) = 2);
	//}

	{
		elastic::flex_buffer_t buffer{ 0 };
		int a = 0;

		EXPECT_FALSE(elastic::to_binary(1, buffer));

		elastic::from_binary(a, buffer);

		EXPECT_TRUE(!buffer.success());
	}

	//{
	//	elastic::flex_buffer_t buffer{};

	//	uint8_t a = 12;

	//	buffer.sputn((uint8_t*)&a, 1);

	//	uint8_t b = 0;

	//	buffer.sgetn((uint8_t*)&b, 1);

	//	EXPECT_TRUE(b == a);
	//}

	//{
	//	elastic::flex_buffer_t buffer{ 0 };

	//	uint8_t a{ 1 };

	//	buffer.sgetn(&a, 1);

	//	EXPECT_TRUE(a == 1);
	//}

	{
		elastic::flex_buffer_t buffer{};

		auto gptr = buffer.wdata();

		buffer.consume(-1);

		EXPECT_TRUE(gptr == buffer.wdata());
	}

	{
		elastic::flex_buffer_t buffer{};
		int a = 2;

		elastic::to_binary(a, buffer);

		elastic::flex_buffer_t buf{};
		buf.swap(buffer);

		int b = 3;
		elastic::to_binary(b, buffer);

		buf.append(buffer);

		EXPECT_TRUE(buf.active() == 512 - 4);
		EXPECT_TRUE(buf.size() == 4);
		
		int c{};
		int d{};

		elastic::from_binary(c, buf);
		elastic::from_binary(d, buf);

		EXPECT_TRUE(c == a);
		EXPECT_TRUE(d == b);
	}
}