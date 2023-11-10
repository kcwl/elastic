#pragma once
#include <array>
#include <string_view>
#include <cstdint>

inline constexpr int32_t PARSE_TIMES = 100000;

enum class parse_type
{
	vec3,
	weapon,
	monster,
	monsters,
	rect32,
	rect32s,
	person,
	persons
};

constexpr std::array<std::string_view, 8> parse_type_str = { "vec3",	  "weapon",	 "monster", "monsters",
														"rect32", "rect32s", "person",	"persons" };