#pragma once
#include "defines.h"

#include <fstream>
#include <string>

namespace elastic
{
	namespace compiler
	{
		class file_descriptor
		{
		public:
			file_descriptor()
				: read_file_stream_()
				, input_file_name_()
				, multi_key_words_()
			{}

		public:
			bool read_file(const std::string& file_name);

		private:
			bool read_to_spilt(std::string& value, char sp);

			//bool read_structure(reflactor_structure& impl);

			//bool read_struct_head(reflactor_structure& impl);

			//bool read_struct_body(reflactor_structure& impl);

			bool read_command(reflactor_structure& rs);

			bool choose_state(int current, reflactor_structure& rs);

			//void read_note_dir(reflactor_structure& rs, note_dir way);

			note read_note();

			bool check_key_word(const std::string& value);

			bool check_file_suffix(const std::string& file_name);

			bool read_keyword(std::string& keyword, bool ignore_note = false);

			bool read_message(reflactor_structure& rs);

			bool read_package(reflactor_structure& rs);

		private:
			std::ifstream read_file_stream_;

			std::string input_file_name_;

			std::vector<reflactor_structure> multi_key_words_;
		};
	} // namespace compiler
} // namespace elastic