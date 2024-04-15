#pragma once
#include <elastic.hpp>

struct person
{
	int age;
	std::string name;

	ELASTIC_ACCESS(age, name);
};

struct animal
{
	int a;
	char b;
	bool c;

	ELASTIC_ACCESS(a, b, c);
};

struct person_body_request
{
	bool sex;
	std::vector<uint8_t> role_data;
	double mana;
	float hp;
	int32_t age;
	int64_t money;
	std::string name;
	uint32_t back_money;
	uint64_t crc;

	void swap(person_body_request& other)
	{
		std::swap(sex, other.sex);
		std::swap(role_data, other.role_data);
		std::swap(mana, other.mana);
		std::swap(hp, other.hp);
		std::swap(age, other.age);
		std::swap(money, other.money);
		std::swap(name, other.name);
		std::swap(back_money, other.back_money);
		std::swap(crc, other.crc);
	}

private:
	friend class elastic::access;

	template <typename _Archive>
	void serialize(_Archive& ar)
	{
		ar& sex;
		ar& role_data;
		ar& mana;
		ar& hp;
		ar& age;
		ar& money;
		ar& name;
		ar& back_money;
		ar& crc;
	}
};

TEST(io, elastic_type)
{
	{
		char a_in = (std::numeric_limits<char>::max)();

		elastic::flex_buffer_t buf{};

		elastic::to_binary(a_in, buf);

		char a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		int8_t a_in = (std::numeric_limits<int8_t>::max)();

		elastic::flex_buffer_t buf{};

		elastic::to_binary(a_in, buf);

		int8_t a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		uint8_t a_in = (std::numeric_limits<uint8_t>::max)();

		elastic::flex_buffer_t buf{};

		elastic::to_binary(a_in, buf);

		uint8_t a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		int16_t a_in = (std::numeric_limits<int16_t>::max)();

		elastic::flex_buffer_t buf{};

		elastic::to_binary(a_in, buf);

		int16_t a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}
	{
		uint16_t a_in = (std::numeric_limits<uint16_t>::max)();

		elastic::flex_buffer_t buf{};

		elastic::to_binary(a_in, buf);

		uint16_t a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		int32_t a_in = (std::numeric_limits<int32_t>::max)();

		elastic::flex_buffer_t buf{};

		elastic::to_binary(a_in, buf);

		int32_t a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		uint32_t a_in = (std::numeric_limits<uint32_t>::max)();

		elastic::flex_buffer_t buf{};

		elastic::to_binary(a_in, buf);

		uint32_t a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		int64_t a_in = (std::numeric_limits<int64_t>::max)();

		elastic::flex_buffer_t buf{};

		elastic::to_binary(a_in, buf);

		int64_t a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		uint64_t a_in = (std::numeric_limits<uint64_t>::max)();

		elastic::flex_buffer_t buf{};

		elastic::to_binary(a_in, buf);

		uint64_t a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		int64_t a_in = -10;

		elastic::flex_buffer_t buf{};

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

		elastic::flex_buffer_t buf{};

		color a_in;

		elastic::to_binary(1, buf);

		elastic::from_binary(a_in, buf);

		EXPECT_TRUE(a_in == color::red);
	}

	{
		std::string a_in = "hello world!";

		elastic::flex_buffer_t buf{};

		elastic::to_binary(a_in, buf);

		std::string a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		bool a_in = false;

		elastic::flex_buffer_t buf{};

		elastic::to_binary(a_in, buf);

		bool a_out = true;

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		std::vector<std::byte> a_in = { std::byte('1'), std::byte('2'), std::byte('3'), std::byte('4'),
										std::byte('5') };

		elastic::flex_buffer_t buf{};

		elastic::to_binary(a_in, buf);

		std::vector<std::byte> a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		std::vector<person> a_in{};
		person per = { 1, "Lancy" };

		a_in.push_back(per);
		a_in.push_back(per);
		a_in.push_back(per);

		elastic::flex_buffer_t buf{};

		elastic::to_binary(a_in, buf);

		std::vector<person> pers_copy{};

		elastic::from_binary(pers_copy, buf);

		auto size = pers_copy.size();

		for (std::size_t i = 0; i < size; ++i)
		{
			EXPECT_TRUE(pers_copy[i].age == a_in[i].age && pers_copy[i].name == a_in[i].name);
		}
	}

	{
		std::vector<animal> animals{};
		animals.push_back({ 1, '2', 0 });
		animals.push_back({ 2, '2', 0 });
		animals.push_back({ 3, '2', 0 });

		std::vector<animal> a_in{};

		elastic::flex_buffer_t buf{};

		elastic::to_binary(a_in, buf);

		elastic::from_binary(a_in, buf);

		auto size = a_in.size();

		for (std::size_t i = 0; i < size; ++i)
		{
			EXPECT_TRUE(a_in[i].a == animals[i].a && a_in[i].b == animals[i].b &&
						a_in[i].c == animals[i].c);
		}
	}

	{
		double a_in = 1.2;

		elastic::flex_buffer_t buf{};

		elastic::to_binary(a_in, buf);

		double a_out;

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		float a_in = 2.4f;

		elastic::flex_buffer_t buf{};

		elastic::to_binary(a_in, buf);

		float a_out;

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		person_body_request req{};
		req.sex = true;
		req.role_data.push_back('a');
		req.mana = 12.2;
		req.hp = 100.1f;
		req.age = 1;
		req.money = -2;
		req.name = "hello";
		req.back_money = 10000000;
		req.crc = 2;

		elastic::flex_buffer_t buffer{};
		elastic::to_binary(req, buffer);

		person_body_request req1{};

		elastic::from_binary(req1, buffer);
	}
}