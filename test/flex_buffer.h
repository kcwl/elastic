#pragma once
#include <elastic.hpp>
#include <fstream>

TEST(buffer, construct)
{
	{
		elastic::flex_buffer_t buffer{};

		int a = 5;

		elastic::to_binary(a, buffer);

		elastic::flex_buffer_t buffer_c(buffer.begin(), buffer.end());

		EXPECT_TRUE(buffer_c.size() == 1);

		EXPECT_TRUE(*buffer_c.wdata() == 10);
	}

	{
		unsigned char a[4] = { '0', '0', '0', '5' };

		elastic::flex_buffer_t buffer(std::span{ a });

		EXPECT_TRUE(buffer.size() == 4);

		EXPECT_TRUE(*buffer.wdata() == '0');
		buffer.consume(1);
		EXPECT_TRUE(*buffer.wdata() == '0');
		buffer.consume(1);
		EXPECT_TRUE(*buffer.wdata() == '0');
		buffer.consume(1);
		EXPECT_TRUE(*buffer.wdata() == '5');
	}

	{
		elastic::flex_buffer_t buffer{};

		int a = 5;

		elastic::to_binary(a, buffer);

		elastic::flex_buffer_t buffer_c{};
		buffer_c = std::move(buffer);

		EXPECT_TRUE(buffer.size() == 0);

		EXPECT_TRUE(buffer_c.size() == 1);

		EXPECT_TRUE(*buffer_c.wdata() == 10);
	}

	{
		unsigned char* a = new unsigned char[4];
		a[0] = '0';
		a[1] = '0';
		a[2] = '0';
		a[3] = '5';

		elastic::flex_buffer_t buffer(a, 4);

		EXPECT_TRUE(buffer.size() == 4);

		EXPECT_TRUE(*buffer.wdata() == '0');
		buffer.consume(1);
		EXPECT_TRUE(*buffer.wdata() == '0');
		buffer.consume(1);
		EXPECT_TRUE(*buffer.wdata() == '0');
		buffer.consume(1);
		EXPECT_TRUE(*buffer.wdata() == '5');

		delete[] a;
	}
}

TEST(buffer, function)
{
	{
		const elastic::flex_buffer_t buffer{};

		EXPECT_TRUE(buffer.max_size() == 4096);

		EXPECT_TRUE(buffer.active() == 4096);

		EXPECT_TRUE(buffer.size() == 0);

		EXPECT_TRUE(*buffer.data() == 0);

		EXPECT_TRUE(*buffer.wdata() == 0);

		EXPECT_TRUE(*buffer.rdata() == 0);

		EXPECT_TRUE(*buffer.begin() == *buffer.data());
	}

	{
		elastic::flex_buffer_t m_buffer{};

		m_buffer.resize(8192);

		EXPECT_TRUE(m_buffer.max_size() == 8192);

		elastic::to_binary(1, m_buffer);
		elastic::to_binary(2, m_buffer);
		elastic::to_binary(3, m_buffer);
		elastic::to_binary(4, m_buffer);
		elastic::to_binary(5, m_buffer);

		m_buffer.consume(4);

		m_buffer.resize(3);

		EXPECT_TRUE(m_buffer.max_size() == 3);

		m_buffer.clear();

		EXPECT_TRUE(m_buffer.max_size() == 0);
	}

	{
		elastic::flex_buffer_t buffer{};

		elastic::to_binary(1, buffer);

		int a = 0;

		elastic::from_binary(a, buffer);

		EXPECT_TRUE(a == 1);

		buffer.ensure();
		buffer.normalize();

		EXPECT_TRUE(buffer.size() == 0 && buffer.active() == 4096);
	}

	{
		elastic::flex_buffer_t buffer{};
		buffer.commit(4090);

		buffer.ensure();

		EXPECT_TRUE(buffer.max_size() == (4096 + 4096));
	}

	{
		elastic::flex_buffer_t buffer{};
		buffer.normalize();
	}

	{
		elastic::flex_buffer_t buffer{};

		buffer.commit(4096);

		char a = '2';

		EXPECT_TRUE(!elastic::to_binary(a, buffer));
	}

	{
		elastic::flex_buffer_t buffer{};

		buffer.commit(5);

		buffer.pubseekpos(3, std::ios::out);

		EXPECT_TRUE(buffer.size() == 2);
	}

	{
		elastic::flex_buffer_t buffer{};

		buffer.pubseekoff(5, std::ios::cur, std::ios::in);

		EXPECT_TRUE(buffer.size() == 5);

		buffer.pubseekoff(3, std::ios::beg, std::ios::out);

		buffer.pubseekoff(1, std::ios::cur, std::ios::out);

		EXPECT_TRUE(buffer.size() == 1);

		EXPECT_TRUE(buffer.pubseekoff(1, std::ios::end, std::ios::in) == 4095);
	}

	{
		elastic::flex_buffer_t buffer{};
		int a = 0;

		EXPECT_TRUE(!elastic::from_binary(a, buffer));
	}

	{
		elastic::flex_buffer_t buffer{ 1 };

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