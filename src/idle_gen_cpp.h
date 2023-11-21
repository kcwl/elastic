#pragma once
#include "code_generator.h"
#include "defines.h"

#include <fstream>
#include <map>
#include <queue>

using namespace std::string_view_literals;

namespace elastic
{
	namespace compiler
	{
		namespace cpp
		{
			class generate_cpp : public code_generator
			{
			public:
				generate_cpp() = default;

				virtual ~generate_cpp() = default;

			public:
				virtual bool generate(const std::string& input_file, const std::string& output_dir) override;

			private:
				bool write_file();

				void write_struct_declare();

				void write_struct_declare_header();

				void write_struct_def();

				void begin_write_class(const reflactor_structure& rs);

				void write_friend_class(const std::string& class_name);

				void write_struct_impl(const reflactor_structure& s);

				void write_pod_t();

				void write_construct(const std::string& class_name);

				void write_member_func(const reflactor_structure& s);

				void write_member_impl();

				void end_write_class(const reflactor_structure& rs);

				void real_write_file(std::ofstream& ofs);

			private:
				

				std::string output_file_name_;

				std::ofstream write_h_stream_;

				std::ofstream write_cpp_stream_;

				

				std::vector<std::string> lines;
			};
		} // namespace cpp
	}	  // namespace compiler
} // namespace elastic
