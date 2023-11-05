#pragma once
#include "code_generator.h"
#include "part.h"

#include <fstream>
#include <map>

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

				bool write_h();

				std::string read_to_spilt(std::ifstream& ifs, const char sp);
				
				void read_structure();

				keyword read_struct_head(reflactor_structure& impl);

				void read_struct_body(reflactor_structure& impl);

				void write_header();

				void write_namespace_begin(std::ofstream& fs);

				void write_namespace_end(std::ofstream& fs);

				void write_struct();

				void write_meta_member(reflactor_structure& iter);

				void write_serialize();

				void write_parse();

				std::string advance_tab();

				keyword check_key_word(const std::string& value);

				std::pair<std::string,keyword> get_type_name();

				

			private:
				std::ifstream read_file_stream_;

				std::string input_file_name_;

				std::ofstream write_h_stream_;

				std::vector<reflactor_structure> multi_key_words_;
			};
		} // namespace cpp
	}	  // namespace compiler
} // namespace elastic
