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
} // namespace

namespace elastic
{
	namespace compiler
	{
		namespace cpp
		{
			bool generate_cpp::generate(const std::string& input_file, const std::string& output_dir)
			{
				input_file_name_ = input_file;

				read_file_stream_.open(input_file, std::ios::binary | std::ios::in);
				write_h_stream_.open(input_file + ".h", std::ios::binary | std::ios::out);

				if (!read_file())
					return false;

				write_h();

				// write_cpp();

				return true;
			}

			bool generate_cpp::read_file()
			{
				// if (!read_file_stream_.is_open())
				//	return false;

				// while (!read_file_stream_.eof())
				//{
				//	auto [key, res] = get_type_name();

				//	file_status status{};

				//	switch (res)
				//	{
				//	case elastic::compiler::keyword::single:
				//		{
				//			status = read_single_key();
				//		}
				//		break;
				//	case elastic::compiler::keyword::multi:
				//		{
				//			status = read_multi_key();
				//		}
				//		break;
				//	case elastic::compiler::keyword::error:
				//		break;
				//	default:
				//		break;
				//	}

				//	if (status != file_status::success)
				//		return false;
				//}

				while (!read_file_stream_.eof())
				{
					read_structure();
				}

				return true;
			}

			bool generate_cpp::write_h()
			{
				if (!write_h_stream_.is_open())
					return false;

				write_header();

				write_namespace_begin(write_h_stream_);

				write_struct();

				write_namespace_end(write_h_stream_);

				return true;
			}

			std::string generate_cpp::read_to_spilt(std::ifstream& ifs, const char sp)
			{
				std::array<char, 1024> lines;

				while (true)
				{
					if (ifs.peek() == '\r' || ifs.peek() == '\n')
						ifs.get();
					else
						break;
				}

				ifs.get(&lines[0], 1024, sp);

				ifs.get();

				return std::string(lines.data());
			}

			void generate_cpp::read_structure()
			{
				reflactor_structure impl{};

				keyword status = read_struct_head(impl);

				switch (status)
				{
				case elastic::compiler::keyword::single:
					{
						impl.prio_ = static_cast<int>(priority::single);
					}
					break;
				case elastic::compiler::keyword::multi:
					{
						impl.prio_ = static_cast<int>(priority::multi);
						read_struct_body(impl);
					}
					break;
				case elastic::compiler::keyword::error:
					return;
				default:
					break;
				}

				multi_key_words_.push(impl);
			}

			keyword generate_cpp::read_struct_head(reflactor_structure& impl)
			{
				impl.type_ = read_to_spilt(read_file_stream_, ' ');

				auto status = check_key_word(impl.type_);

				if (status == keyword::error)
					return status;
				else if (status == keyword::single)
				{
					impl.name_ = read_to_spilt(read_file_stream_, ';');
				}
				else
				{
					auto name_and_number = read_to_spilt(read_file_stream_, '{');

					common::trip(name_and_number, '\r', '\n', '\t', ' ');

					auto pos = name_and_number.find('=');

					if (pos == std::string::npos)
					{
						impl.name_ = name_and_number;
					}
					else
					{
						pos = name_and_number.find('=');

						impl.name_ = name_and_number.substr(0, pos);

						impl.number_ = name_and_number.substr(pos + 1);
					}
				}

				return status;
			}

			void generate_cpp::read_struct_body(reflactor_structure& impl)
			{
				while (true)
				{
					auto key = read_to_spilt(read_file_stream_, ' ');

					if (key.empty())
						continue;

					if (*key.begin() == '}')
					{
						int size = static_cast<int>(key.size() - 1);

						read_file_stream_.seekg(-size, std::ios::cur);

						break;
					}

					common::trip(key, '\t', '\r', '\n', ' ');

					impl.structs_.push_back({});

					auto& i = impl.structs_.back();

					if (check_key_word(key) == keyword::single)
					{
						i.type_ = key;

						i.name_ = read_to_spilt(read_file_stream_, ';');
					}
					else
					{
						read_struct_head(i);
					}
				}
			}

			void generate_cpp::write_header()
			{
				// for (auto iter : hf_.list_)
				//{
				//	write_h_stream_ << "#include <" + iter + ">" << enter;
				// }

				// write_h_stream_ << "#include <elastic.hpp>" << enter;

				// write_h_stream_ << enter;
			}

			void generate_cpp::write_namespace_begin(std::ofstream& fs)
			{
				// if (namespace_file_.empty())
				//	return;

				// fs << "namespace " << namespace_file_ << enter << "{" << enter;
				// layout_++;
			}

			void generate_cpp::write_namespace_end(std::ofstream& fs)
			{
				// if (namespace_file_.empty())
				//	return;

				// fs << "}" << enter;
			}

			void generate_cpp::write_struct()
			{
				// for (auto& iter : sf_.list_)
				//{
				//	write_h_stream_ << advance_tab() << iter.type_ << " " << iter.name_;

				//	switch (iter.kt_)
				//	{
				//	case keyword_type::enumclass_t:
				//		{
				//			if (!iter.default_type_.empty())
				//			{
				//				write_h_stream_ << advance_tab() << " : " << iter.default_type_;
				//			}
				//		}
				//		break;
				//	case keyword_type::message_t:
				//		{
				//			// write_h_stream_ << " : public parse";
				//		}
				//		break;
				//	default:
				//		break;
				//	}

				//	write_h_stream_ << enter;

				//	write_h_stream_ << advance_tab() << "{" << enter;

				//	if (iter.kt_ == keyword_type::message_t)
				//	{
				//		layout_++;
				//		for (auto& impl : iter.member_list_)
				//		{
				//			write_h_stream_ << advance_tab();
				//			if (impl.property_ == "optional")
				//			{
				//				write_h_stream_ << "elastic::optional<" + impl.type_ + "> ";
				//			}
				//			else if (impl.property_ == "require")
				//			{
				//				write_h_stream_ << "elastic::require<" + impl.type_ + "> ";
				//			}
				//			else if (impl.property_ == "repeated")
				//			{
				//				write_h_stream_ << "elastic::vector<" + impl.type_ + "> ";
				//			}
				//			else
				//			{
				//				write_h_stream_ << impl.type_ + " ";
				//			}

				//			write_h_stream_ << impl.name_ << ";" << double_enter;
				//		}
				//	}

				//	write_meta_member(iter);

				//	//----
				//	write_serialize();

				//	write_parse();

				//	layout_--;

				//	write_h_stream_ << advance_tab() << "};" << double_enter;
				//}
			}

			void generate_cpp::write_meta_member(reflactor_structure& iter)
			{
				// write_h_stream_ << advance_tab() << "static auto member()" << enter;
				// write_h_stream_ << advance_tab() << "{" << enter;

				// layout_++;
				// write_h_stream_ << advance_tab() << "struct member_impl" << enter;
				// write_h_stream_ << advance_tab() << "{" << enter;
				// layout_++;
				// write_h_stream_ << advance_tab() << "static constexpr auto member_list() { return std::make_tupe(";
				// std::size_t count = 1;
				// for (auto& impl : iter.member_list_)
				//{
				//	write_h_stream_ << "std::pair{ " << impl.flag_ << R"(, ")" << impl.name_ << R"(" })";
				//	if (count != iter.member_list_.size())
				//		write_h_stream_ << ", ";
				// }

				// write_h_stream_ << ")};" << enter;
				// write_h_stream_ << advance_tab() << "static constexpr auto index() { return " << iter.type_ << "; }"
				//				<< enter;

				// write_h_stream_ << advance_tab() << "static constexpr auto require() { std::array<int,"
				//				<< iter.require_list_.size() << "> { ";

				// count = 1;
				// for (auto& req : iter.require_list_)
				//{
				//	write_h_stream_ << req;

				//	if (count == iter.require_list_.size())
				//		continue;

				//	write_h_stream_ << ",";
				//}

				// write_h_stream_ << "};}" << enter;

				// layout_--;
				// write_h_stream_ << advance_tab() << "};" << double_enter;

				// write_h_stream_ << advance_tab() << "return member_impl{};" << enter;
				// layout_--;
				// write_h_stream_ << advance_tab() << "}" << double_enter;
			}

			void generate_cpp::write_serialize()
			{
				// write_h_stream_ << advance_tab() << "auto serialize_to_array() -> std::pair<const char*,std::size_t>"
				//				<< enter;

				// write_h_stream_ << advance_tab() << "{" << enter;
				// layout_++;
				// write_h_stream_ << advance_tab() << "elastic::iostream ios{};" << enter;
				// write_h_stream_ << advance_tab() << "ios << *this;" << double_enter;
				// write_h_stream_ << advance_tab() << "return { (char*)ios.data(), ios.size() };" << enter;
				// layout_--;
				// write_h_stream_ << advance_tab() << "}" << double_enter;

				// write_h_stream_ << advance_tab() << "std::string serialize_as_string()" << enter;
				// write_h_stream_ << advance_tab() << "{" << enter;
				// layout_++;
				// write_h_stream_ << advance_tab() << "auto [buf,size] = serialize_to_array();" << double_enter;
				// write_h_stream_ << advance_tab() << "return std::string(buf, size);" << enter;
				// layout_--;
				// write_h_stream_ << advance_tab() << "}" << double_enter;
			}

			void generate_cpp::write_parse()
			{
				// write_h_stream_ << advance_tab() << "bool parse_from_array(const void* buf, int size)" << enter;
				// write_h_stream_ << advance_tab() << "{" << enter;
				// layout_++;
				// write_h_stream_ << advance_tab()
				//				<< "elastic::iostream ios(std::span<uint8_t>((uint8_t*)buf, (uint8_t*)buf + size));"
				//				<< double_enter;
				// write_h_stream_ << advance_tab() << "ios >> *this;" << double_enter;
				// write_h_stream_ << advance_tab() << "return true;" << double_enter;
				// layout_--;
				// write_h_stream_ << advance_tab() << "}" << double_enter;

				// write_h_stream_ << advance_tab() << "bool parse_from_string(const std::string& buf)" << enter;
				// write_h_stream_ << advance_tab() << "{" << enter;
				// layout_++;
				// write_h_stream_ << advance_tab() << "elastic::iostream ios{ std::span<const char>{buf} };" << enter
				//				<< enter;
				// write_h_stream_ << advance_tab() << "ios >> *this;" << double_enter;
				// write_h_stream_ << advance_tab() << "return true;" << enter;
				// layout_--;
				// write_h_stream_ << advance_tab() << "}" << enter;
			}

			std::string generate_cpp::advance_tab()
			{
				std::string result{};
				// for (std::size_t i = 0; i < layout_; ++i)
				//{
				//	result += tab;
				// }

				return result;
			}

			keyword generate_cpp::check_key_word(const std::string& value)
			{
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

			std::pair<std::string, keyword> generate_cpp::get_type_name()
			{
				auto key = read_to_spilt(read_file_stream_, ' ');

				auto status = check_key_word(key);

				if (status == keyword::error)
					return std::pair<std::string, keyword>{ {}, keyword::error };

				return { key, status };
			}

		} // namespace cpp
	}	  // namespace compiler
} // namespace elastic