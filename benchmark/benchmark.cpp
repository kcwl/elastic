// benchmark.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "elc.hpp"
#include "protobuf.hpp"

#include <iostream>

template <parse_type Value>
void serialize(elc e, protobuf p)
{
	std::cout << "=====================================================\n";

	e.serialize<Value>();
	 p.seriliaze<Value>();

	std::cout << "=====================================================\n\n";
}

template <parse_type Value>
void deserialize(elc e, protobuf p)
{
	std::cout << "=====================================================\n";

	e.deserialize<Value>();
	 p.deseriliaze<Value>();

	std::cout << "=====================================================\n\n";
}

int main()
{
	std::cout << "benchmark : elastic and protobuf\n\n\n";

	elc els{};

	protobuf proto{};

	serialize<parse_type::vec3>(els, proto);
	serialize<parse_type::weapon>(els, proto);
	serialize<parse_type::monster>(els, proto);
	serialize<parse_type::monsters>(els, proto);
	serialize<parse_type::rect32>(els, proto);
	serialize<parse_type::rect32s>(els, proto);
	serialize<parse_type::person>(els, proto);
	serialize<parse_type::persons>(els, proto);

	std::cout << "\n\n\n\n";

	deserialize<parse_type::vec3>(els, proto);
	deserialize<parse_type::weapon>(els, proto);
	deserialize<parse_type::monster>(els, proto);
	deserialize<parse_type::monsters>(els, proto);
	deserialize<parse_type::rect32>(els, proto);
	deserialize<parse_type::rect32s>(els, proto);
	deserialize<parse_type::person>(els, proto);
	deserialize<parse_type::persons>(els, proto);

	std::cout << "those are the benchmark results\n";
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧:
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
