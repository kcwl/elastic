#pragma once
#include <string>
#include <vector>

namespace elastic
{
	namespace compiler
	{
		enum class keyword_type : int
		{
			none_t,
			include_t,
			namespace_t,
			enumclass_t,
			message_t,
		};

		enum class file_status
		{
			success,
			format_error,
		};

		enum class keyword
		{
			single,
			multi,
			error
		};

		enum class priority
		{
			single,
			multi
		};

		struct header_file
		{
			std::vector<std::string> list_;
		};

		struct reflactor_structure
		{
			bool operator<(const reflactor_structure other) const
			{
				return other.prio_ < prio_;
			}

			std::string number_;

			std::string type_;

			std::string name_;

			int32_t prio_;

			std::vector<reflactor_structure> structs_;
		};
	} // namespace compiler
} // namespace elastic