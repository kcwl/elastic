#include "idle_gen_cpp.h"

#include "common.hpp"

#include <array>
#include <filesystem>
#include <string_view>

namespace
{
	const std::map<std::string, std::string> type_pair = {
		{ "int32", "int32_t" },	  { "int64", "int64_t" },	  { "string", "std::string" }, { "bool", "bool" },
		{ "uint32", "uint32_t" }, { "uint64", "uint64_t" },	  { "bytes", "bytes" },		   { "float", "float" },
		{ "double", "double" },	  { "fixed32", "fixed32_t" }, { "fixed64", "fixed64_t" },
	};

	std::string get_type_name(const std::string& type)
	{
		auto pos = type.find("map");

		if (pos == std::string::npos)
			return type_pair.at(type);

		pos = type.find_first_of("<");
		auto d_pos = type.find_first_of(",");

		if (d_pos == std::string::npos)
			return {};

		auto left_type = type.substr(pos + 1, d_pos - pos - 1);
		auto right_type = type.substr(d_pos + 1);

		if (left_type.empty() || right_type.empty())
			return {};

		right_type = right_type.substr(0, right_type.size() - 1);

		return "std::map<" + type_pair.at(left_type) + "," + type_pair.at(right_type) + ">";
	}

} // namespace

namespace elastic
{
	namespace compiler
	{
		namespace cpp
		{
			bool generate_cpp::generate(const std::string& input_file, const std::string& output_dir)
			{
				auto pos = input_file.find_last_of("/\\");

				if (pos == std::string::npos)
					input_file_name_ = input_file;
				else
					input_file_name_ = input_file.substr(pos + 1);

				output_file_name_ = output_dir;

				read_file_stream_.open(input_file, std::ios::binary | std::ios::in);
				write_h_stream_.open(input_file + ".h", std::ios::binary | std::ios::out);
				write_cpp_stream_.open(input_file + ".cpp", std::ios::binary | std::ios::out);

				if (!read_file())
					return false;

				if (!write_file())
					return false;

				return true;
			}

			bool generate_cpp::read_file()
			{
				while (!read_file_stream_.eof())
				{
					reflactor_structure impl{};

					auto cur = read_file_stream_.peek();

					if (cur == '\r' || cur == '\n' || cur == ' ')
					{
						read_file_stream_.get();

						continue;
					}

					choose_state(cur, impl);

					multi_key_words_.push_back(impl);
				}

				return true;
			}

			bool generate_cpp::read_to_spilt(std::string& value, const char sp)
			{
				std::array<char, 1024> temp_line;

				read_file_stream_.get(&temp_line[0], 1024, sp);

				read_file_stream_.get();

				value = temp_line.data();

				return true;
			}

			bool generate_cpp::read_structure(reflactor_structure& impl)
			{
				bool result = read_struct_head(impl);

				if (!result)
					return false;

				read_note_dir(impl, note_dir::left);

				result = read_struct_body(impl);

				if (!result)
					return false;

				read_note_dir(impl, note_dir::right);

				return true;
			}

			bool generate_cpp::read_struct_head(reflactor_structure& impl)
			{
				while (!read_file_stream_.eof())
				{
					auto cur = read_file_stream_.peek();

					if (cur == ' ')
					{
						read_file_stream_.get();

						continue;
					}

					if (cur != '\r' && cur != '\n' && cur != '\t')
						break;

					read_file_stream_.get();
				}

				if (!read_to_spilt(impl.type_, ' '))
					return false;

				if (!check_key_word(impl.type_))
					return false;

				std::string name_and_number{};

				read_to_spilt(name_and_number, '{');

				trip(name_and_number, '\r', '\n', '\t', ' ');

				auto pos = name_and_number.find('=');

				if (pos == std::string::npos)
				{
					impl.name_ = name_and_number;
				}
				else
				{
					impl.name_ = name_and_number.substr(0, pos);

					auto note_pos = name_and_number.find('/');

					if (note_pos == std::string::npos)
						impl.number_ = name_and_number.substr(pos + 1);
					else
					{
						impl.number_ = name_and_number.substr(pos + 1, note_pos - pos - 1);

						impl.note_.content_ = std::string(" // ") + name_and_number.substr(note_pos + 2);

						impl.note_.type_ = note_type::double_slash;
					}
				}

				return true;
			}

			bool generate_cpp::read_struct_body(reflactor_structure& impl)
			{
				while (!read_file_stream_.eof())
				{
					auto cur = read_file_stream_.peek();
					if (cur == '}')
					{
						read_file_stream_.get();
						break;
					}

					if (cur == '\t' || cur == '\n' || cur == ' ')
					{
						read_file_stream_.get();

						continue;
					}

					impl.structs_.push_back({});

					auto& i = impl.structs_.back();

					choose_state(cur, i);
				}

				return !read_file_stream_.eof();
			}

			bool generate_cpp::read_commond(reflactor_structure& rs)
			{
				if (!read_to_spilt(rs.type_, ' '))
					return false;

				if (!check_key_word(rs.type_))
					return false;

				if (!read_to_spilt(rs.name_, ';'))
					return false;

				rs.note_ = read_note();

				return true;
			}

			void generate_cpp::choose_state(int current, reflactor_structure& rs)
			{
				switch (current)
				{
				case '/':
					{
						rs.note_ = read_note();
					}
					break;
				case 'm':
					{
						read_file_stream_.get();

						auto cur = read_file_stream_.peek();

						read_file_stream_.seekg(-1, std::ios::cur);

						if (cur == 'e')
							read_structure(rs);
						else
							read_commond(rs);
					}
					break;
				default:
					{
						read_commond(rs);
					}
					break;
				}
			}

			void generate_cpp::read_note_dir(reflactor_structure& rs, note_dir way)
			{
				switch (way)
				{
				case elastic::note_dir::left:
					rs.left_note_ = read_note();
					break;
				case elastic::note_dir::right:
					rs.right_note_ = read_note();
					break;
				default:
					break;
				}
			}

			note generate_cpp::read_note()
			{
				note n{};

				std::string space{};

				while (!read_file_stream_.eof())
				{
					auto cur = read_file_stream_.peek();

					read_file_stream_.get();

					if (cur == '\r' || cur == '\n')
						break;

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

				return n;
			}

			bool generate_cpp::write_file()
			{
				if (!write_h_stream_.is_open())
					return false;

				write_struct_declare();

				real_write_file(write_h_stream_);

				if (!write_cpp_stream_.is_open())
					return false;

				write_struct_def();

				real_write_file(write_cpp_stream_);

				return true;
			}

			void generate_cpp::write_struct_declare()
			{
				write_struct_declare_header();

				bool has_namespace = false;

				for (auto& s : multi_key_words_)
				{
					if (s.type_ == "package")
					{
						has_namespace = true;

						lines.push_back("namespace " + s.name_ + s.note_.content_);
						lines.push_back("{");
					}
					else if (s.type_ == "message")
					{
						begin_write_class(s);

						write_friend_class(s.name_);

						write_struct_impl(s);

						write_pod_t();

						write_construct(s.name_);

						write_member_func(s);

						write_member_impl();

						end_write_class(s);
					}
					else
					{
						lines.push_back(s.note_.content_);
					}
				}

				if (has_namespace)
				{
					lines.push_back("}");
				}
			}

			void generate_cpp::write_struct_declare_header()
			{
				lines.push_back("#pragma once");
				lines.push_back("#include <elastic.hpp>");
				lines.push_back({});
			}

			void generate_cpp::write_struct_def()
			{
				lines.clear();

				lines.push_back("#include \"" + input_file_name_ + ".h\"");
				lines.push_back({});

				bool has_namespace = false;

				for (auto& s : multi_key_words_)
				{
					if (s.type_ == "package")
					{
						lines.push_back("namespace " + s.name_);
						lines.push_back("{");

						has_namespace = true;
					}
					else if (s.type_ == "message")
					{
						for (auto& mem : s.structs_)
						{
							if (mem.type_.empty())
								continue;

							auto type = get_type_name(mem.type_);

							if (type.empty())
								continue;

							lines.push_back(type + " " + s.name_ + "::" + mem.name_ + "() const");
							lines.push_back("{");
							lines.push_back("return impl." + mem.name_ + ";");
							lines.push_back("}");
							lines.push_back({});

							lines.push_back("void " + s.name_ + "::set_" + mem.name_ + "(const " + type + "& " +
											mem.name_ + ")");
							lines.push_back("{");
							lines.push_back("impl." + mem.name_ + " = " + mem.name_ + ";");
							lines.push_back("}");
						}

						lines.push_back({});
						lines.push_back("elastic::message_pod& " + s.name_ + "::internal_type()");
						lines.push_back("{");
						lines.push_back("return impl;");
						lines.push_back("}");
					}
				}

				if (has_namespace)
				{
					lines.push_back("}");
				}
			}

			bool generate_cpp::check_key_word(const std::string& value)
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

			void generate_cpp::begin_write_class(const reflactor_structure& rs)
			{
				auto base_type = std::format("elastic::message_lite<{}>", rs.name_);

				lines.push_back("class " + rs.name_ + " final : public " + base_type + rs.note_.content_);

				lines.push_back("{" + rs.left_note_.content_);
			}

			void generate_cpp::write_friend_class(const std::string& class_name)
			{
				auto base_type = std::format("elastic::message_lite<{}>", class_name);

				lines.push_back("friend class " + base_type + ";");
			}

			void generate_cpp::write_struct_impl(const reflactor_structure& s)
			{
				lines.push_back("struct member_impl : elastic::message_pod");
				lines.push_back("{");
				lines.push_back("REFLECT_DEFINE");
				lines.push_back("(");

				for (auto& mem : s.structs_)
				{
					if (mem.type_.empty())
					{
						lines.push_back(mem.note_.content_);

						continue;
					}

					auto type = get_type_name(mem.type_);

					if (type.empty())
						continue;

					lines.push_back(type + " " + mem.name_ + ";" + mem.note_.content_);
				}

				lines.push_back(")");
				lines.push_back({});
				lines.push_back("bool operator==(const member_impl& other) const");
				lines.push_back("{");
				lines.push_back("return");

				int count = 0;
				for (auto& mem : s.structs_)
				{
					if (mem.name_.empty() || mem.type_.empty())
						continue;

					count++;

					std::string line = tab + mem.name_ + " == other." + mem.name_;

					if (count != s.structs_.size())
						line += " &&";
					else
					{
						line += ";";
					}

					lines.push_back(line);
				}

				lines.push_back("}");

				lines.push_back("private:");

				lines.push_back(tab + "friend class elastic::access;");
				lines.push_back({});
				lines.push_back(tab + "template<typename _Archive>");
				lines.push_back(tab + "void serialize(_Archive& ar)");
				lines.push_back(tab + "{");

				for (auto& mem : s.structs_)
				{
					if (mem.name_.empty() || mem.type_.empty())
						continue;

					lines.push_back(tab + tab + "ar& " + mem.name_ + ";");
				}
				lines.push_back(tab + "}");

				lines.push_back("};");
				lines.push_back({});
			}

			void generate_cpp::write_pod_t()
			{
				lines.push_back("public:");
				lines.push_back(tab + "using pod_t = member_impl;");
				lines.push_back({});
			}

			void generate_cpp::write_construct(const std::string& class_name)
			{
				lines.push_back("public:");
				lines.push_back(tab + class_name + "() = default;");
				lines.push_back(tab + "virtual ~" + class_name + "() = default;");
				lines.push_back({});

				lines.push_back("public:");
				lines.push_back(tab + "bool operator==(const " + class_name + "& other) const");
				lines.push_back(tab + "{");
				lines.push_back(tab + tab + "return impl == other.impl;");
				lines.push_back(tab + "}");
				lines.push_back({});
			}

			void generate_cpp::write_member_func(const reflactor_structure& s)
			{
				lines.push_back("public:");

				for (auto& mem : s.structs_)
				{
					if (mem.type_.empty())
						continue;

					auto type = get_type_name(mem.type_);

					if (type.empty())
						continue;

					lines.push_back(tab + type + " " + mem.name_ + "() const;" + crlf);
					lines.push_back(tab + "void set_" + mem.name_ + "(const " + type + "& " + mem.name_ + ");" + crlf);
					lines.push_back(tab + "void clear_" + mem.name_ + "();" + crlf);
				}
			}

			void generate_cpp::write_member_impl()
			{
				lines.push_back("public:");
				lines.push_back(tab + "virtual elastic::message_pod& internal_type() final;" + crlf);

				lines.push_back("private:");
				lines.push_back(tab + "member_impl impl;");
			}

			void generate_cpp::end_write_class(const reflactor_structure& rs)
			{
				lines.push_back("};" + rs.right_note_.content_);
			}

			void generate_cpp::real_write_file(std::ofstream& ofs)
			{
				std::string space{};
				for (auto& line : lines)
				{
					if (!line.empty())
					{
						if (line[0] == ')' || line[0] == '}')
							space.pop_back();
					}

					ofs << space << line << crlf;

					if (line[0] == '{' || line[0] == '(')
						space += tab;
				}
			}
		} // namespace cpp
	}	  // namespace compiler
} // namespace elastic