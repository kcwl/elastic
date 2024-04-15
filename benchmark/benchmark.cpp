// benchmark.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "benchmark/benchmark.h"
#include "elastic.hpp"
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

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
		ar & sex;
		ar & role_data;
		ar & mana;
		ar & hp;
		ar & age;
		ar & money;
		ar & name;
		ar & back_money;
		ar & crc;
	}
};

static void elastic_serialize(benchmark::State& state)
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
	// int req = -15;

	elastic::flex_buffer_t buffer{};

	for (auto _ : state)
	{
		elastic::to_binary(req, buffer);
	}
}

BENCHMARK(elastic_serialize);

static void elastic_deserialize(benchmark::State& state)
{
	person_body_request req{};
	req.sex = false;
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

	for (auto _ : state)
		elastic::from_binary(req1, buffer);
}

BENCHMARK(elastic_deserialize);

BENCHMARK_MAIN();

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧:
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
