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
				bool read_file();

				bool write_file();

				bool read_to_spilt(std::string& value, const char sp);

				bool read_structure(reflactor_structure& impl);

				bool read_struct_head(reflactor_structure& impl);

				bool read_struct_body(reflactor_structure& impl);

				bool read_commond(reflactor_structure& rs);

				void choose_state(int current, reflactor_structure& rs);

				void read_note_dir(reflactor_structure& rs, note_dir way);

				void write_struct_declare();

				void write_struct_def();

				bool check_key_word(const std::string& value);

				void begin_write_class(const reflactor_structure& rs, const std::string& space);

				void write_friend_class(const std::string& class_name, const std::string& space);

				void write_struct_impl(const reflactor_structure& s, const std::string& space);

				void write_pod_t(const std::string& space);

				void write_construct(const std::string& class_name, const std::string& space);

				void write_member_func(const reflactor_structure& s, const std::string& space);

				void write_member_impl(const std::string& space);

				void end_write_class(const reflactor_structure& rs, const std::string& space);

				note read_note();

			private:
				std::ifstream read_file_stream_;

				std::string input_file_name_;

				std::string output_file_name_;

				std::ofstream write_h_stream_;

				std::ofstream write_cpp_stream_;

				std::vector<reflactor_structure> multi_key_words_;
			};
		} // namespace cpp
	}	  // namespace compiler
} // namespace elastic
