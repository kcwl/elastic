#include "file_descriptor.h"

#include "common.hpp"

#include <iostream>

namespace elastic
{
	namespace compiler
	{
		bool file_descriptor::read_file(const std::string& file_name)
		{
			if (!check_file_suffix(file_name))
			{
				std::cout << "file format error, maybe suffix is not \".mpr\"!";

				return false;
			}

			read_file_stream_.open(file_name, std::ios::in | std::ios::binary);
			if (!read_file_stream_.is_open())
			{
				std::cout << "maybe file is not exist!";

				return false;
			}

			while (!read_file_stream_.eof())
			{
				reflactor_structure impl{};

				auto cur = read_file_stream_.peek();

				if (cur == '/')
				{
					impl.note_ = read_note();
				}
				else
				{
					if (!read_keyword(impl.type_))
						return false;

					if (impl.type_ == "message")
						read_message(impl);
					else if (impl.type_ == "package")
						read_package(impl);
					else
						continue;
				}

				multi_key_words_.push_back(impl);
			}

			return true;
		}

		bool file_descriptor::read_to_spilt(std::string& value, char sp)
		{
			std::array<char, 1024> temp_line;

			auto cur = read_file_stream_.peek();

			read_file_stream_.get(&temp_line[0], 1024, sp);

			read_file_stream_.get();

			value = temp_line.data();

			return true;
		}

		//bool file_descriptor::read_structure(reflactor_structure& impl)
		//{
		//	bool result = read_struct_head(impl);

		//	if (!result)
		//		return false;

		//	read_note_dir(impl, note_dir::left);

		//	result = read_struct_body(impl);

		//	if (!result)
		//		return false;

		//	read_note_dir(impl, note_dir::right);

		//	return true;
		//}

		//bool file_descriptor::read_struct_head(reflactor_structure& impl)
		//{
		//	while (!read_file_stream_.eof())
		//	{
		//		auto cur = read_file_stream_.peek();

		//		if (cur == ' ')
		//		{
		//			read_file_stream_.get();

		//			continue;
		//		}

		//		if (cur != '\r' && cur != '\n' && cur != '\t')
		//			break;

		//		read_file_stream_.get();
		//	}

		//	if (!read_to_spilt(impl.type_, ' '))
		//		return false;

		//	if (!check_key_word(impl.type_))
		//		return false;

		//	std::string name_and_number{};

		//	read_to_spilt(name_and_number, '{');

		//	trip(name_and_number, '\r', '\n', '\t', ' ');

		//	auto pos = name_and_number.find('=');

		//	if (pos == std::string::npos)
		//	{
		//		impl.name_ = name_and_number;
		//	}
		//	else
		//	{
		//		impl.name_ = name_and_number.substr(0, pos);

		//		auto note_pos = name_and_number.find('/');

		//		if (note_pos == std::string::npos)
		//			impl.number_ = name_and_number.substr(pos + 1);
		//		else
		//		{
		//			impl.number_ = name_and_number.substr(pos + 1, note_pos - pos - 1);

		//			impl.note_.content_ = std::string(" // ") + name_and_number.substr(note_pos + 2);

		//			impl.note_.type_ = note_type::double_slash;
		//		}
		//	}

		//	return true;
		//}

		//bool file_descriptor::read_struct_body(reflactor_structure& impl)
		//{
		//	while (!read_file_stream_.eof())
		//	{
		//		auto cur = read_file_stream_.peek();
		//		if (cur == '}')
		//		{
		//			read_file_stream_.get();
		//			break;
		//		}

		//		if (cur == '\t' || cur == '\n' || cur == ' ')
		//		{
		//			read_file_stream_.get();

		//			continue;
		//		}

		//		impl.structs_.push_back({});

		//		auto& i = impl.structs_.back();

		//		choose_state(cur, i);
		//	}

		//	return !read_file_stream_.eof();
		//}

		bool file_descriptor::read_command(reflactor_structure& rs)
		{
			//if (!read_to_spilt(rs.type_, ' '))
			//	return false;

			//if (!check_key_word(rs.type_))
			//	return false;

			if (!read_to_spilt(rs.name_, ';'))
				return false;

			rs.note_ = read_note();

			while (!read_file_stream_.eof())
			{
				auto cur = read_file_stream_.peek();

				if (cur != '\r' && cur != '\n' && cur != '\t')
					break;

				read_file_stream_.get();
			}

			return true;
		}

		bool file_descriptor::choose_state(int current, reflactor_structure& rs)
		{
			bool result = true;
			switch (current)
			{
			case '/':
				{
					rs.note_ = read_note();
				}
				break;
			default:
				{
					result = read_command(rs);
				}
				break;
			}

			return result;
		}

		//void file_descriptor::read_note_dir(reflactor_structure& rs, note_dir way)
		//{
		//	switch (way)
		//	{
		//	case elastic::note_dir::left:
		//		rs.left_note_ = read_note();
		//		break;
		//	case elastic::note_dir::right:
		//		rs.right_note_ = read_note();
		//		break;
		//	default:
		//		break;
		//	}
		//}

		note file_descriptor::read_note()
		{
			note n{};

			std::string space{};

			while (!read_file_stream_.eof())
			{
				auto cur = read_file_stream_.peek();

				read_file_stream_.get();

				if (cur != '/')
				{
					space += static_cast<char>(cur);
					continue;
				}

				cur = read_file_stream_.get();

				if (cur == '*')
				{
					read_to_spilt(n.content_, '/');

					n.content_ = space + std::string("/") + n.content_ + std::string("/");

					n.type_ = note_type::slash;
				}
				else if (cur == '/')
				{
					read_to_spilt(n.content_, '\r');

					n.content_ = space + std::string("//") + n.content_;

					read_file_stream_.get();

					n.type_ = note_type::double_slash;
				}

				break;
			}

			while (true)
			{
				auto cur = read_file_stream_.peek();

				if (cur != '\r' && cur != '\n')
					break;

				n.content_ += static_cast<char>(cur);

				read_file_stream_.get();
			}

			return n;
		}

		bool file_descriptor::check_key_word(const std::string& value)
		{
			if (value.empty())
				return false;

			if (value[0] == '/')
				return false;

			std::string temp_value = value;

			if (temp_value.find("map") != std::string::npos)
				temp_value = "map";

			auto iter_multi = std::find_if(multi_key_words.begin(), multi_key_words.end(),
										   [&](auto&& key) { return key == temp_value; });

			if (iter_multi != multi_key_words.end())
				return true;

			auto iter_single = std::find_if(single_key_words.begin(), single_key_words.end(),
											[&](auto&& key) { return key == temp_value; });

			if (iter_single != single_key_words.end())
				return true;

			return false;
		}

		bool file_descriptor::check_file_suffix(const std::string& file_name)
		{
			auto slash_pos = file_name.find_last_of('/\\');

			slash_pos == std::string::npos ? slash_pos = 0 : slash_pos+=1;

			auto pos = file_name.find_last_of(".");
			if (pos == std::string::npos)
				return false;

			input_file_name_ = file_name.substr(slash_pos, pos - slash_pos);

			return true;
		}

		bool file_descriptor::read_keyword(std::string& keyword, bool ignore_note)
		{
			if (!read_to_spilt(keyword, ' '))
				return false;

			auto size = keyword.size();
			std::size_t i = 0;
			while (i < size - 1)
			{
				if (keyword[i] == '/')
				{
					std::size_t pos{};

					if (keyword[i + 1] == '/')
					{
						pos = keyword.find_first_of('\r\n');

						if (pos == std::string::npos)
							return false;
					}
					else if (keyword[i + 1] == '*')
					{
						std::size_t pos{};

						while (pos < keyword.size())
						{
							pos = keyword.find('*', pos);

							if (pos == std::string::npos)
								return false;

							pos++;

							if (pos < keyword.size() - 1 && keyword[pos + 1] == '/')
								break;
						}

						pos += 2;
					}

					if (!ignore_note)
					{
						multi_key_words_.push_back({});

						auto& back = multi_key_words_.back();

						back.note_.content_ = keyword.substr(0, pos);
					}

					keyword = keyword.substr(pos + 1);

					i = 0;

					size = keyword.size();

					continue;
				}

				trip(keyword, '\r', '\n', '\t', ' ');

				break;
			}

			return true;
		}

		bool file_descriptor::read_message(reflactor_structure& rs)
		{
			std::string name_and_number{};

			read_to_spilt(name_and_number, '{');

			trip(name_and_number, '\r', '\n', '\t', ' ');

			auto pos = name_and_number.find('=');

			if (pos == std::string::npos)
			{
				return false;
			}

			rs.name_ = name_and_number.substr(0, pos);

			auto note_pos = name_and_number.find('/');

			if (note_pos == std::string::npos)
				rs.number_ = name_and_number.substr(pos + 1);
			else
			{
				rs.number_ = name_and_number.substr(pos + 1, note_pos - pos - 1);
			}

			read_file_stream_.get();

			while (true)
			{
				rs.structs_.push_back({});

				auto& impl = rs.structs_.back();

				if (!read_keyword(impl.type_, true))
					return false;

				if (impl.type_[0] == '}')
				{
					rs.structs_.pop_back();
					break;
				}

				read_command(impl);
			}

			return true;
		}

		bool file_descriptor::read_package(reflactor_structure& rs)
		{
			return read_command(rs);
		}
	} // namespace compiler
} // namespace elastic