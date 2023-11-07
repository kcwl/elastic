#include "idle_gen_cpp.h"

#include "common.hpp"

#include <array>
#include <filesystem>
#include <string_view>

namespace
{
	constexpr static std::string_view tab = "\t"sv;
	constexpr static std::string_view enter = "\n"sv;
	constexpr static std::string_view double_enter = "\n\n"sv;

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

					if (!read_structure(impl))
						continue;

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

			std::string generate_cpp::read_to_spilt(std::ifstream& ifs, const char sp)
			{
				std::array<char, 1024> lines;

				while (!ifs.eof())
				{
					if (ifs.peek() == '\r' || ifs.peek() == '\n')
						ifs.get();
					else
						break;
				}

				if (ifs.eof())
					return {};

				ifs.get(&lines[0], 1024, sp);

				ifs.get();

				return std::string(lines.data());
			}

			bool generate_cpp::read_structure(reflactor_structure& impl)
			{
				bool result = true;

				keyword status = read_struct_head(impl);

				switch (status)
				{
				case elastic::keyword::single:
					{
						impl.name_ = read_to_spilt(read_file_stream_, ';');

						impl.note_ = read_note();
					}
					break;
				case elastic::keyword::multi:
					{
						auto name_and_number = read_to_spilt(read_file_stream_, '{');

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

								impl.note_ = std::string(" // ") + name_and_number.substr(note_pos + 2);
							}
						}

						read_struct_body(impl);
					}
					break;
				case elastic::keyword::note:
					{
						impl.note_ = read_note();
					}
					break;
				case elastic::keyword::error:
				{
						result = false;
				}
				default:
					break;
				}

				return result;
			}

			keyword generate_cpp::read_struct_head(reflactor_structure& impl)
			{
				while (!read_file_stream_.eof())
				{
					auto cur = read_file_stream_.peek();

					if (cur == '/')
						return keyword::note;

					if (cur == ' ')
					{
						read_file_stream_.get();

						continue;
					}
						
					if (cur != '\r' && cur != '\n' && cur !='\t')
						break;

					read_file_stream_.get();
				}

				impl.type_ = read_to_spilt(read_file_stream_, ' ');

				auto status = check_key_word(impl.type_);

				return status;
			}

			void generate_cpp::read_struct_body(reflactor_structure& impl)
			{
				while (true)
				{
					auto cur = read_file_stream_.peek();
					if (cur == '}')
					{
						read_file_stream_.get();
						break;
					}

					impl.structs_.push_back({});

					auto& i = impl.structs_.back();

					auto status = read_struct_head(i);

					switch (status)
					{
					case keyword::single:
					{
							i.name_ = read_to_spilt(read_file_stream_, ';');

							i.note_ = read_note();
					}
					break;
					case keyword::note:
					{
							i.note_ = read_note();
					}
					break;
					case keyword::multi:
					{
							i.structs_.push_back({});
							auto& next_i = i.structs_.back();

							read_structure(next_i);
					}
					default:
					break;
					}
				}
			}

			void generate_cpp::write_struct_declare()
			{
				write_h_stream_ << "#pragma once\n";
				write_h_stream_ << "#include <elastic.hpp>\n\n";

				bool has_namespace = false;

				int count = 0;
				for (auto& s : multi_key_words_)
				{
					count++;

					if (s.type_ == "package")
					{
						has_namespace = true;

						write_h_stream_ << "namespace " << s.name_;

						if (!s.note_.empty())
							write_h_stream_ << s.note_ << "\r";

						write_h_stream_ << "\n{\n";
					}
					else if (s.type_ == "message")
					{
						std::string class_format_space = "";

						if (has_namespace)
							class_format_space = "\t";

						std::string member_format_space = class_format_space + "\t";

						begin_write_class(s.name_, s.note_, class_format_space);

						write_friend_class(s.name_, member_format_space);

						write_struct_impl(s, member_format_space);

						write_pod_t(class_format_space);

						write_construct(s.name_, class_format_space);

						write_member_func(s, class_format_space);

						write_member_impl(class_format_space);

						end_write_class(class_format_space);
					}
					else
					{
						std::string class_format_space = "";

						if (has_namespace)
							class_format_space = "\t";

						write_h_stream_ << class_format_space <<s.note_ << '\r';
					}

					if (count == multi_key_words_.size())
						write_h_stream_ << "\n";
				}
				if (has_namespace)
				{
					write_h_stream_ << "\n}";
				}
			}

			void generate_cpp::write_struct_def()
			{
				write_cpp_stream_ << "#include \"" << input_file_name_ << ".h\"\n\n";

				bool has_namespace = false;

				std::string class_format_space{};

				for (auto& s : multi_key_words_)
				{
					if (s.type_ == "package")
					{
						has_namespace = true;

						write_cpp_stream_ << "namespace " << s.name_ << "\n{\n";
					}
					else if (s.type_ == "message")
					{
						if (has_namespace)
							class_format_space = "\t";

						int count = 0;

						for (auto& mem : s.structs_)
						{
							if (mem.type_.empty())
								continue;

							auto type = type_pair.at(mem.type_);

							if (type.empty())
								continue;

							count++;

							write_cpp_stream_ << class_format_space << "const " << type << "& " << mem.name_ << "()\n";
							write_cpp_stream_ << class_format_space << "{\n";
							write_cpp_stream_ << class_format_space << "\treturn impl." << mem.name_ << ";\n";
							write_cpp_stream_ << class_format_space << "}\n\n";

							write_cpp_stream_ << class_format_space << "const " << type << "& " << mem.name_
											  << "() const\n";
							write_cpp_stream_ << class_format_space << "{\n";
							write_cpp_stream_ << class_format_space << "\treturn impl." << mem.name_ << ";\n";
							write_cpp_stream_ << class_format_space << "}\n\n";

							write_cpp_stream_ << class_format_space << "void set_" << mem.name_ << "(const " << type
											  << "& " << mem.name_ << ")\n";
							write_cpp_stream_ << class_format_space << "{\n";
							write_cpp_stream_ << class_format_space << "\timpl." << mem.name_ << " = " << mem.name_
											  << ";\n";
							write_cpp_stream_ << class_format_space << "}\n";

							if (count != s.structs_.size())
								write_cpp_stream_ << "\n";
						}
					}
				}

				if (has_namespace)
				{
					write_cpp_stream_ << "}";
				}
			}

			keyword generate_cpp::check_key_word(const std::string& value)
			{
				if (value.empty())
					return keyword::error;

				if (value[0] == '/')
					return keyword::note;

				auto iter_multi = std::find_if(multi_key_words.begin(), multi_key_words.end(),
											   [&](auto&& key) { return key == value; });

				if (iter_multi != multi_key_words.end())
					return keyword::multi;

				auto iter_single = std::find_if(single_key_words.begin(), single_key_words.end(),
												[&](auto&& key) { return key == value; });

				if (iter_single != single_key_words.end())
					return keyword::single;

				return keyword::error;
			}

			void generate_cpp::begin_write_class(const std::string& class_name, const std::string& note, const std::string& space)
			{
				auto base_type = std::format("elastic::message_lite<{}>", class_name);

				write_h_stream_ << space << "class " << class_name << " final : public " << base_type << note;

				write_h_stream_ << "\n" << space << "{";
			}

			void generate_cpp::write_friend_class(const std::string& class_name, const std::string& space)
			{
				auto base_type = std::format("elastic::message_lite<{}>", class_name);

				write_h_stream_ << '\n' << space << "friend class " << base_type << ";\n\n";
			}

			void generate_cpp::write_struct_impl(const reflactor_structure& s, const std::string& space)
			{
				write_h_stream_ << space << "struct member_impl : elastic::message_pod\n";
				write_h_stream_ << space << "{";

				write_h_stream_ <<'\n'<< space << "REFLECT_DEFINE(";

				std::string struct_format_space = space + "\t";

				int count = 0;
				for (auto& mem : s.structs_)
				{
					if (mem.type_.empty())
					{
						write_h_stream_ << mem.note_ << "\r\n";
						continue;
					}

					auto type = type_pair.at(mem.type_);

					if (type.empty())
						continue;

					count++;

					write_h_stream_ << '\n' << struct_format_space << type << " " << mem.name_ << ";" << mem.note_;

					if (count != s.structs_.size())
						write_h_stream_ << "\n";
				}

				write_h_stream_ << space << ")\n";
				write_h_stream_ << space << "};\n\n";
			}

			void generate_cpp::write_pod_t(const std::string& space)
			{
				write_h_stream_ << space << "public :\n";

				write_h_stream_ << space << "\tusing pod_t = member_impl;\n\n";
			}

			void generate_cpp::write_construct(const std::string& class_name, const std::string& space)
			{
				write_h_stream_ << space << "public:\n";

				write_h_stream_ << space << "\t" << class_name << "() =default;\n\n";

				write_h_stream_ << space << "\t"
								<< "virtual ~" << class_name << "() = default;\n\n";
			}

			void generate_cpp::write_member_func(const reflactor_structure& s, const std::string& space)
			{
				write_h_stream_ << space << "public:\n";

				auto member_format_space = space + "\t";

				for (auto& mem : s.structs_)
				{
					if (mem.type_.empty())
						continue;

					auto type = type_pair.at(mem.type_);

					if (type.empty())
						continue;

					write_h_stream_ << member_format_space << "const " << type << "& " << mem.name_ << "();\n\n";

					write_h_stream_ << member_format_space << "const " << type << "& " << mem.name_ << "(); const\n\n";

					write_h_stream_ << member_format_space << "void set_" << mem.name_ << "(const " << type << "& "
									<< mem.name_ << ");\n\n";
				}
			}

			void generate_cpp::write_member_impl(const std::string& space)
			{
				write_h_stream_ << space << "private:\n";
				write_h_stream_ << space << "\tvirtual elastic::message_pod& internal_type() final;\n\n ";

				write_h_stream_ << space << "private:\n";
				write_h_stream_ << space << "\tmember_impl impl;\n";
			}

			void generate_cpp::end_write_class(const std::string& space)
			{
				write_h_stream_ << space << "};";
			}

			std::string generate_cpp::read_note()
			{
				std::string space{};
				std::string result{};
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
						result = space + std::string("/") + read_to_spilt(read_file_stream_, '/') + std::string("/");
					}
					else if (cur == '/')
					{
						result = space + std::string("//") + read_to_spilt(read_file_stream_, '\r');

						read_file_stream_.get();
					}

					break;
				}

				return result;
			}
		} // namespace cpp
	}	  // namespace compiler
} // namespace elastic