#pragma once
#include <boost/test/unit_test_suite.hpp>
#include <elastic.hpp>
#include <fstream>

BOOST_AUTO_TEST_SUITE(buffer)

BOOST_AUTO_TEST_CASE(construct)
{
	{
		elastic::flex_buffer_t buffer{};

		int a = 5;

		elastic::to_binary(a, buffer);

		elastic::flex_buffer_t buffer_c(buffer.begin(), buffer.end());

		BOOST_CHECK(buffer_c.size() == 1);

		BOOST_CHECK(*buffer_c.wdata() == 10);
	}

	{
		unsigned char a[4] = { '0', '0', '0', '5' };

		elastic::flex_buffer_t buffer(std::span{ a });

		BOOST_CHECK(buffer.size() == 4);

		BOOST_CHECK(*buffer.wdata() == '0');
		buffer.consume(1);
		BOOST_CHECK(*buffer.wdata() == '0');
		buffer.consume(1);
		BOOST_CHECK(*buffer.wdata() == '0');
		buffer.consume(1);
		BOOST_CHECK(*buffer.wdata() == '5');
	}

	{
		elastic::flex_buffer_t buffer{};

		int a = 5;

		elastic::to_binary(a, buffer);

		elastic::flex_buffer_t buffer_c(std::move(buffer));

		BOOST_CHECK(buffer == elastic::flex_buffer_t{ 0 });

		BOOST_CHECK(buffer_c.size() == 1);

		BOOST_CHECK(*buffer_c.wdata() == 10);
	}

	{
		unsigned char* a = new unsigned char[4];
		a[0] = '0';
		a[1] = '0';
		a[2] = '0';
		a[3] = '5';

		elastic::flex_buffer_t buffer(a, 4);

		BOOST_CHECK(buffer.size() == 4);

		BOOST_CHECK(*buffer.wdata() == '0');
		buffer.consume(1);
		BOOST_CHECK(*buffer.wdata() == '0');
		buffer.consume(1);
		BOOST_CHECK(*buffer.wdata() == '0');
		buffer.consume(1);
		BOOST_CHECK(*buffer.wdata() == '5');

		delete[] a;
	}
}

BOOST_AUTO_TEST_CASE(function)
{
	{
		const elastic::flex_buffer_t buffer{};

		BOOST_CHECK(buffer.max_size() == 4096);

		BOOST_CHECK(buffer.active() == 4096);

		BOOST_CHECK(buffer.size() == 0);

		BOOST_CHECK(*buffer.data() == 0);

		BOOST_CHECK(*buffer.wdata() == 0);

		BOOST_CHECK(*buffer.rdata() == 0);

		BOOST_CHECK(*buffer.begin() == *buffer.data());
	}

	{
		elastic::flex_buffer_t m_buffer{};

		m_buffer.resize(8192);

		BOOST_CHECK(m_buffer.max_size() == 8192);

		elastic::to_binary(1, m_buffer);
		elastic::to_binary(2, m_buffer);
		elastic::to_binary(3, m_buffer);
		elastic::to_binary(4, m_buffer);
		elastic::to_binary(5, m_buffer);

		m_buffer.consume(4);

		m_buffer.resize(3);

		BOOST_CHECK(m_buffer.max_size() == 3);

		m_buffer.clear();

		BOOST_CHECK(m_buffer.max_size() == 0);
	}

	{
		elastic::flex_buffer_t buffer{};

		elastic::to_binary(1, buffer);

		int a = 0;

		elastic::from_binary(a, buffer);

		BOOST_CHECK(a == 1);

		buffer.ensure();
		buffer.normalize();

		BOOST_CHECK(buffer.size() == 0 && buffer.active() == 4096);
	}

	{
		elastic::flex_buffer_t buffer{};
		buffer.commit(4090);

		buffer.ensure();

		BOOST_CHECK(buffer.max_size() == (4096 + 4096));
	}

	{
		elastic::flex_buffer_t buffer{};
		buffer.normalize();
	}

	{
		elastic::flex_buffer_t buffer{};

		buffer.commit(4096);

		char a = '2';

		BOOST_CHECK(!elastic::to_binary(a, buffer));
	}

	{
		elastic::flex_buffer_t buffer{};

		buffer.commit(5);

		buffer.pubseekpos(3, std::ios::out);

		BOOST_CHECK(buffer.size() == 2);
	}

	{
		elastic::flex_buffer_t buffer{};

		buffer.pubseekoff(5, std::ios::cur, std::ios::in);

		BOOST_CHECK(buffer.size() == 5);

		buffer.pubseekoff(3, std::ios::beg, std::ios::out);

		buffer.pubseekoff(1, std::ios::cur, std::ios::out);

		BOOST_CHECK(buffer.size() == 1);

		BOOST_CHECK(buffer.pubseekoff(1, std::ios::end, std::ios::in) == 4095);
	}

	{
		elastic::flex_buffer_t buffer{};
		int a = 0;

		BOOST_CHECK(!elastic::from_binary(a, buffer));
	}

	{
		elastic::flex_buffer_t buffer{1};

		elastic::to_binary(1, buffer);

		elastic::flex_buffer_t buf{};

		elastic::to_binary(2, buf);

		buffer.append(std::move(buf));

		int a = 0;
		int b = 0;

		elastic::from_binary(a, buffer);
		elastic::from_binary(b, buffer);

		BOOST_CHECK(a == 1 && b == 2);
	}
}

BOOST_AUTO_TEST_SUITE_END()