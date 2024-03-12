#pragma once
#include <elastic.hpp>

TEST(io, elastic_type)
{
	{
		elastic::flex_buffer_t buf;

		char a_in = (std::numeric_limits<char>::max)();

		elastic::to_binary(a_in, buf);

		char a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		elastic::flex_buffer_t buf;

		int8_t a_in = (std::numeric_limits<int8_t>::max)();

		elastic::to_binary(a_in, buf);

		int8_t a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		elastic::flex_buffer_t buf;

		uint8_t a_in = (std::numeric_limits<uint8_t>::max)();

		elastic::to_binary(a_in, buf);

		uint8_t a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		elastic::flex_buffer_t buf;

		int16_t a_in = (std::numeric_limits<int16_t>::max)();

		elastic::to_binary(a_in, buf);

		int16_t a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}
	{
		elastic::flex_buffer_t buf;

		uint16_t a_in = (std::numeric_limits<uint16_t>::max)();

		elastic::to_binary(a_in, buf);

		uint16_t a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		elastic::flex_buffer_t buf;

		int32_t a_in = (std::numeric_limits<int32_t>::max)();

		elastic::to_binary(a_in, buf);

		int32_t a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		elastic::flex_buffer_t buf;

		uint32_t a_in = (std::numeric_limits<uint32_t>::max)();

		elastic::to_binary(a_in, buf);

		uint32_t a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		elastic::flex_buffer_t buf;

		int64_t a_in = (std::numeric_limits<int64_t>::max)();

		elastic::to_binary(a_in, buf);

		int64_t a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		elastic::flex_buffer_t buf;

		uint64_t a_in = (std::numeric_limits<uint64_t>::max)();

		elastic::to_binary(a_in, buf);

		uint64_t a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		elastic::flex_buffer_t buf;

		int64_t a_in =  -10;

		elastic::to_binary(a_in, buf);

		int64_t a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		enum class color
		{
			red = 1,
			blue = 2
		};

		elastic::flex_buffer_t buf;

		elastic::to_binary(color::red, buf);

		color cr{};

		elastic::from_binary(cr, buf);

		EXPECT_TRUE(cr == color::red);
	}

	{
		elastic::flex_buffer_t buf;

		std::string a_in = "hello world!";

		elastic::to_binary(a_in, buf);

		std::string a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		elastic::flex_buffer_t buf;

		bool a_in = false;

		elastic::to_binary(a_in, buf);

		bool a_out = true;

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		//elastic::flex_buffer_t buf;

		//std::vector<std::byte> a_in = { std::byte('1'), std::byte('2'), std::byte('3'), std::byte('4'),
		//								std::byte('5') };

		//elastic::to_binary(a_in, buf);

		//std::vector<std::byte> a_out{};

		//elastic::from_binary(a_out, buf);

		//EXPECT_TRUE(a_in == a_out);
	}

	{
		elastic::flex_buffer_t buf;

		double a_in = 1.2;

		elastic::to_binary(a_in, buf);

		double a_out;

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		elastic::flex_buffer_t buf;

		float a_in = 2.4f;

		elastic::to_binary(a_in, buf);

		float a_out;

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		std::optional<int> value{ 1 };

		elastic::flex_buffer_t buf;

		elastic::to_binary(value, buf);

		std::optional<int> value1;

		elastic::from_binary(value1, buf);

		EXPECT_TRUE(value.value() == value1.value());
	}

	//{
	//	elastic::flex_buffer_t buf;

	//	std::vector<std::byte> a_in = { std::byte(1), std::byte(2), std::byte(3), std::byte(4), std::byte(5) };

	//	elastic::to_binary(a_in, buf);

	//	std::vector<std::byte> a_out{};

	//	elastic::from_binary(a_out, buf);

	//	EXPECT_TRUE(a_in == a_out);
	//}
}