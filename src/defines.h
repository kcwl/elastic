#pragma once
#include <string>
#include <vector>
#include <array>
#include <string_view>

using namespace std::string_view_literals;

namespace elastic
{
	enum class file_status
	{
		success,
		format_error,
	};

	enum class keyword
	{
		single,
		multi,
		note,
		error
	};

	struct reflactor_structure
	{
		std::string note_;

		std::string number_;

		std::string type_;

		std::string name_;

		std::vector<reflactor_structure> structs_;
	};

	constexpr static std::array<std::string_view, 1> multi_key_words = { "message"sv };

	constexpr static std::array<std::string_view, 12> single_key_words = { "package"sv, "int32"sv,	 "int64"sv,
																		   "uint32"sv,	"uint64"sv,	 "float"sv,
																		   "double"sv,	"fixed32"sv, "fixed64"sv,
																		   "bool"sv,	"bytes"sv,	 "string"sv };
} // namespace elastic