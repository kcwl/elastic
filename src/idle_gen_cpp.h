#pragma once
#include "code_generator.h"
#include "part.h"

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
				bool read_file();

				bool write_file();

				std::string read_to_spilt(std::ifstream& ifs, const char sp);
				
				void read_structure();

				keyword read_struct_head(reflactor_structure& impl);

				void read_struct_body(reflactor_structure& impl);

				void write_struct_declare();

				void write_struct_def();

				keyword check_key_word(const std::string& value);

				void begin_write_class(const std::string& class_name, const std::string& space);

				void write_friend_class(const std::string& class_name, const std::string& space);

				void write_struct_impl(const reflactor_structure& s, const std::string& space);

				void write_pod_t(const std::string& space);

				void write_construct(const std::string& class_name, const std::string& space);

				void write_member_func(const reflactor_structure& s, const std::string& space);

				void write_member_impl(const std::string& space);

				void end_write_class(const std::string& space);

			private:
				std::ifstream read_file_stream_;

				std::string input_file_name_;

				std::string output_file_name_;

				std::ofstream write_h_stream_;

				std::ofstream write_cpp_stream_;

				std::priority_queue<reflactor_structure> multi_key_words_;
			};
		} // namespace cpp
	}	  // namespace compiler
} // namespace elastic
