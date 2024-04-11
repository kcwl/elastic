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

		auto buf = elastic::to_binary(a_in);

		char a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		int8_t a_in = (std::numeric_limits<int8_t>::max)();

		auto buf = elastic::to_binary(a_in);

		int8_t a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		uint8_t a_in = (std::numeric_limits<uint8_t>::max)();

		auto buf = elastic::to_binary(a_in);

		uint8_t a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		int16_t a_in = (std::numeric_limits<int16_t>::max)();

		auto buf = elastic::to_binary(a_in);


		int16_t a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}
	{
		uint16_t a_in = (std::numeric_limits<uint16_t>::max)();

		auto buf = elastic::to_binary(a_in);

		uint16_t a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		int32_t a_in = (std::numeric_limits<int32_t>::max)();

		auto buf = elastic::to_binary(a_in);

		int32_t a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		uint32_t a_in = (std::numeric_limits<uint32_t>::max)();

		auto buf = elastic::to_binary(a_in);

		uint32_t a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		int64_t a_in = (std::numeric_limits<int64_t>::max)();

		auto buf = elastic::to_binary(a_in);

		int64_t a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		uint64_t a_in = (std::numeric_limits<uint64_t>::max)();

		auto buf = elastic::to_binary(a_in);

		uint64_t a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		int64_t a_in = -10;

		auto buf = elastic::to_binary(a_in);

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

		auto buf = elastic::to_binary(color::red);

		color cr{};

		elastic::from_binary(cr, buf);

		EXPECT_TRUE(cr == color::red);
	}

	{
		std::string a_in = "hello world!";

		auto buf = elastic::to_binary(a_in);

		std::string a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		bool a_in = false;

		auto buf = elastic::to_binary(a_in);

		bool a_out = true;

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		std::vector<std::byte> a_in = { std::byte('1'), std::byte('2'), std::byte('3'), std::byte('4'),
										std::byte('5') };

		auto buf = elastic::to_binary(a_in);

		std::vector<std::byte> a_out{};

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		std::vector<person> pers{};
		person per = { 1, "Lancy" };

		pers.push_back(per);
		pers.push_back(per);
		pers.push_back(per);

		auto buf = elastic::to_binary(pers);

		std::vector<person> pers_copy{};

		elastic::from_binary(pers_copy, buf);

		auto size = pers_copy.size();

		for (std::size_t i = 0; i < size; ++i)
		{
			EXPECT_TRUE(pers_copy[i].age == pers[i].age && pers_copy[i].name == pers[i].name);
		}
	}

	{
		std::vector<animal> animals{};
		animals.push_back({ 1, '2', 0 });
		animals.push_back({ 2, '2', 0 });
		animals.push_back({ 3, '2', 0 });

		std::vector<animal> animal_copys{};

		auto buf = elastic::to_binary(animals);

		elastic::from_binary(animal_copys, buf);

		auto size = animal_copys.size();

		for (std::size_t i = 0; i < size; ++i)
		{
			EXPECT_TRUE(animal_copys[i].a == animals[i].a && animal_copys[i].b == animals[i].b &&
						animal_copys[i].c == animals[i].c);
		}
	}

	{
		double a_in = 1.2;

		auto buf = elastic::to_binary(a_in);

		double a_out;

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}

	{
		float a_in = 2.4f;

		auto buf = elastic::to_binary(a_in);

		float a_out;

		elastic::from_binary(a_out, buf);

		EXPECT_TRUE(a_in == a_out);
	}
}