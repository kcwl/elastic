#include "idle_gen_cpp.h"

#include "common.hpp"

#include <array>
#include <filesystem>
#include <string_view>

namespace
{
	constexpr static std::array<std::string_view, 1> multi_key_words = { "message"sv };

	constexpr static std::array<std::string_view, 12> single_key_words = { "package"sv, "int32"sv,	 "int64"sv,
																		   "uint32"sv,	"uint64"sv,	 "float"sv,
																		   "double"sv,	"fixed32"sv, "fixed64"sv,
																		   "bool"sv,	"bytes"sv,	 "string"sv };

	const std::map<std::string, std::string> type_pair = {
		{ "int32", "int32_t" },
		{ "int64", "int64_t" },
		{ "string", "std::string" },
		{ "bool", "bool" },
		{ "uint32", "uint32_t" },
		{ "uint64", "uint64_t" },
		{ "bytes", "std::vector<uint8_t>" },
		{ "float", "float" },
		{ "double", "double" },
		{ "fixed32", "elastic::fixed<uint32_t>" },
		{ "fixed64", "elastic::fixed<uint64_t>" },
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

			bool generate_cpp::write_file()
			{
				if (!write_h_stream_.is_open())
					return false;

				write_struct_declare();

				if (!write_cpp_stream_.is_open())
					return false;

				write_struct_def();

				return true;
			}

			bool generate_cpp::read_to_spilt(std::string& value, const char sp)
			{
				std::array<char, 1024> lines;

				read_file_stream_.get(&lines[0], 1024, sp);

				read_file_stream_.get();

				value = lines.data();

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

			void generate_cpp::write_struct_declare()
			{
				write_h_stream_ << "#pragma once" << crlf;
				write_h_stream_ << "#include <elastic.hpp>" << crlf << crlf;

				bool has_namespace = false;

				int count = 0;
				for (auto& s : multi_key_words_)
				{
					count++;

					if (s.type_ == "package")
					{
						has_namespace = true;

						write_h_stream_ << "namespace " << s.name_;

						if (!s.note_.content_.empty())
							write_h_stream_ << s.note_.content_;

						if (s.note_.type_ == note_type::slash)
							write_h_stream_ << crlf;

						write_h_stream_ << crlf;

						write_h_stream_ << "{" << crlf;
					}
					else if (s.type_ == "message")
					{
						std::string class_format_space = "";

						if (has_namespace)
							class_format_space = "\t";

						std::string member_format_space = class_format_space + "\t";

						begin_write_class(s, class_format_space);

						write_friend_class(s.name_, member_format_space);

						write_struct_impl(s, member_format_space);

						write_pod_t(class_format_space);

						write_construct(s.name_, class_format_space);

						write_member_func(s, class_format_space);

						write_member_impl(class_format_space);

						end_write_class(s, class_format_space);
					}
					else
					{
						std::string class_format_space = "";

						if (has_namespace)
							class_format_space = "\t";

						write_h_stream_ << class_format_space << s.note_.content_ << crlf;

						if (s.note_.type_ == note_type::slash)
							write_h_stream_ << crlf;
					}
				}
				if (has_namespace)
				{
					write_h_stream_ << crlf << "}";
				}
			}

			void generate_cpp::write_struct_def()
			{
				write_cpp_stream_ << "#include \"" << input_file_name_ << ".h" << crlf <<crlf;

				bool has_namespace = false;

				std::string class_format_space{};

				for (auto& s : multi_key_words_)
				{
					if (s.type_ == "package")
					{
						has_namespace = true;

						write_cpp_stream_ << "namespace " << s.name_ << crlf << "{" <<crlf;
					}
					else if (s.type_ == "message")
					{
						if (has_namespace)
							class_format_space = tab;

						int count = 0;

						for (auto& mem : s.structs_)
						{
							if (mem.type_.empty())
								continue;

							auto type = get_type_name(mem.type_);

							if (type.empty())
								continue;

							count++;

							write_cpp_stream_ << class_format_space << "const " << type << "& " << s.name_
											  << "::" << mem.name_ << "()" << crlf;
							write_cpp_stream_ << class_format_space << "{" << crlf;
							write_cpp_stream_ << class_format_space << tab << "return impl." << mem.name_ << ";"
											  << crlf;
							write_cpp_stream_ << class_format_space << "}" << crlf <<crlf;

							write_cpp_stream_ << class_format_space << "const " << type << "& " << s.name_
											  << "::" << mem.name_ << "() const" << crlf;
							write_cpp_stream_ << class_format_space << "{" << crlf;
							write_cpp_stream_ << class_format_space << tab << "return impl." << mem.name_ << ";"
											  << crlf;
							write_cpp_stream_ << class_format_space << "}" << crlf << crlf;

							write_cpp_stream_ << class_format_space << "void " << s.name_ << "::set_" << mem.name_
											  << "(const " << type << "& " << mem.name_ << ")" << crlf;
							write_cpp_stream_ << class_format_space << "{" << crlf;
							write_cpp_stream_ << class_format_space << tab << "impl." << mem.name_ << " = " << mem.name_
											  << ";" << crlf;
							write_cpp_stream_ << class_format_space << "}" << crlf;

							if (count != s.structs_.size())
								write_cpp_stream_ << crlf;
						}
					}
				}

				if (has_namespace)
				{
					write_cpp_stream_ << "}";
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

			void generate_cpp::begin_write_class(const reflactor_structure& rs, const std::string& space)
			{
				auto base_type = std::format("elastic::message_lite<{}>", rs.name_);

				write_h_stream_ << space << "class " << rs.name_ << " final : public " << base_type << rs.note_.content_
								<< crlf;

				if (rs.note_.type_ == note_type::slash)
					write_h_stream_ << crlf;

				write_h_stream_ << space << "{";

				if (!rs.left_note_.content_.empty())
					write_h_stream_ << rs.left_note_.content_ << crlf;
			}

			void generate_cpp::write_friend_class(const std::string& class_name, const std::string& space)
			{
				auto base_type = std::format("elastic::message_lite<{}>", class_name);

				write_h_stream_ << space << "friend class " << base_type << ";" << crlf << crlf;
			}

			void generate_cpp::write_struct_impl(const reflactor_structure& s, const std::string& space)
			{
				write_h_stream_ << space << "struct member_impl : elastic::message_pod\r\n";
				write_h_stream_ << space << "{";

				write_h_stream_ << crlf << space << "REFLECT_DEFINE(";

				std::string struct_format_space = space + "\t";

				int count = 0;
				for (auto& mem : s.structs_)
				{
					if (mem.type_.empty())
					{
						write_h_stream_ << mem.note_.content_ << crlf;

						if (mem.note_.type_ == note_type::slash)
							write_h_stream_ << crlf;

						continue;
					}

					auto type = get_type_name(mem.type_);

					if (type.empty())
						continue;

					count++;

					write_h_stream_ << crlf << struct_format_space << type << " " << mem.name_ << ";"
									<< mem.note_.content_;

					write_h_stream_ << crlf;
				}

				write_h_stream_ << space << ")" << crlf;
				write_h_stream_ << space << "};" << crlf << crlf;
			}

			void generate_cpp::write_pod_t(const std::string& space)
			{
				write_h_stream_ << space << "public :" << crlf;

				write_h_stream_ << space << "\tusing pod_t = member_impl;" << crlf << crlf;
			}

			void generate_cpp::write_construct(const std::string& class_name, const std::string& space)
			{
				write_h_stream_ << space << "public:\r\n";

				write_h_stream_ << space << "\t" << class_name << "() =default;\r\n\r\n";

				write_h_stream_ << space << "\t"
								<< "virtual ~" << class_name << "() = default;\r\n\r\n";
			}

			void generate_cpp::write_member_func(const reflactor_structure& s, const std::string& space)
			{
				write_h_stream_ << space << "public:\r\n";

				auto member_format_space = space + "\t";

				for (auto& mem : s.structs_)
				{
					if (mem.type_.empty())
						continue;

					auto type = get_type_name(mem.type_);

					if (type.empty())
						continue;

					write_h_stream_ << member_format_space << "const " << type << "& " << mem.name_ << "();\r\n\r\n";

					write_h_stream_ << member_format_space << "const " << type << "& " << mem.name_
									<< "() const;\r\n\r\n";

					write_h_stream_ << member_format_space << "void "
									<< "set_" << mem.name_ << "(const " << type << "& " << mem.name_ << ");\r\n\r\n";
				}
			}

			void generate_cpp::write_member_impl(const std::string& space)
			{
				write_h_stream_ << space << "private:\r\n";
				write_h_stream_ << space << "\tvirtual elastic::message_pod& internal_type() final;\r\n\r\n";

				write_h_stream_ << space << "private:\r\n";
				write_h_stream_ << space << "\tmember_impl impl;\r\n";
			}

			void generate_cpp::end_write_class(const reflactor_structure& rs, const std::string& space)
			{
				write_h_stream_ << space << "};";

				if (!rs.right_note_.content_.empty())
					write_h_stream_ << rs.right_note_.content_;
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
		} // namespace cpp
	}	  // namespace compiler
} // namespace elastic