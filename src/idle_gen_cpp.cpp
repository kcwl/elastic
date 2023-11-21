#include "idle_gen_cpp.h"

#include "common.hpp"
#include "file_descriptor.h"

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
			bool generate_cpp::generate(file_descriptor* input_file, const std::string& output_dir)
			{
				input_file_ptr_ = input_file;

				output_file_name_ = output_dir;

				write_h_stream_.open(input_file_ptr_->file_name() + ".mpr.h", std::ios::binary | std::ios::out);

				if (!write_h_stream_.is_open())
					return false;

				write_cpp_stream_.open(input_file_ptr_->file_name() + ".mpr.cpp", std::ios::binary | std::ios::out);

				if (!write_cpp_stream_.is_open())
					return false;

				if (!write_file())
					return false;

				return true;
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

				for (auto& s : input_file_ptr_->structs())
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

						write_construct(s.name_);

						write_internal_func_declare();

						write_members(s.structs_);

						// write_friend_class(s.name_);

						// write_struct_impl(s);

						// write_pod_t();

						// write_member_func(s);

						end_write_class();
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

				lines.push_back("#include \"" + input_file_ptr_->file_name() + ".mpr.h\"");
				lines.push_back({});

				bool has_namespace = false;

				for (auto& s : input_file_ptr_->structs())
				{
					if (s.type_ == "package")
					{
						lines.push_back("namespace " + s.name_);
						lines.push_back("{");

						has_namespace = true;
					}
					else if (s.type_ == "message")
					{
						write_internal_func_def(s, "from_binary");

						write_internal_func_def(s, "to_binary");
					}
				}

				if (has_namespace)
				{
					lines.push_back("}");
				}
			}

			void generate_cpp::begin_write_class(const reflactor_structure& rs)
			{
				auto base_type = std::format("elastic::message_lite<{}>", rs.name_);

				lines.push_back("class " + rs.name_ + " final : public " + base_type + rs.note_.content_);

				lines.push_back("{");
			}

			// void generate_cpp::write_friend_class(const std::string& class_name)
			//{
			//	auto base_type = std::format("elastic::message_lite<{}>", class_name);

			//	lines.push_back("friend class " + base_type + ";");
			//}

			// void generate_cpp::write_struct_impl(const reflactor_structure& s)
			//{
			//	lines.push_back("struct member_impl : elastic::message_pod");
			//	lines.push_back("{");
			//	lines.push_back("REFLECT_DEFINE");
			//	lines.push_back("(");

			//	for (auto& mem : s.structs_)
			//	{
			//		if (mem.type_.empty())
			//		{
			//			lines.push_back(mem.note_.content_);

			//			continue;
			//		}

			//		auto type = get_type_name(mem.type_);

			//		if (type.empty())
			//			continue;

			//		lines.push_back(type + " " + mem.name_ + ";" + mem.note_.content_);
			//	}

			//	lines.push_back(")");
			//	lines.push_back({});
			//	lines.push_back("bool operator==(const member_impl& other) const");
			//	lines.push_back("{");
			//	lines.push_back("return");

			//	int count = 0;
			//	for (auto& mem : s.structs_)
			//	{
			//		if (mem.name_.empty() || mem.type_.empty())
			//			continue;

			//		count++;

			//		std::string line = tab + mem.name_ + " == other." + mem.name_;

			//		if (count != s.structs_.size())
			//			line += " &&";
			//		else
			//		{
			//			line += ";";
			//		}

			//		lines.push_back(line);
			//	}

			//	lines.push_back("}");

			//	lines.push_back("private:");

			//	lines.push_back(tab + "friend class elastic::access;");
			//	lines.push_back({});
			//	lines.push_back(tab + "template<typename _Archive>");
			//	lines.push_back(tab + "void serialize(_Archive& ar)");
			//	lines.push_back(tab + "{");

			//	for (auto& mem : s.structs_)
			//	{
			//		if (mem.name_.empty() || mem.type_.empty())
			//			continue;

			//		lines.push_back(tab + tab + "ar& " + mem.name_ + ";");
			//	}
			//	lines.push_back(tab + "}");

			//	lines.push_back("};");
			//	lines.push_back({});
			//}

			// void generate_cpp::write_pod_t()
			//{
			//	lines.push_back("public:");
			//	lines.push_back(tab + "using pod_t = member_impl;");
			//	lines.push_back({});
			// }

			void generate_cpp::write_construct(const std::string& class_name)
			{
				lines.push_back("public:");
				lines.push_back(class_name + "() = default;");
				lines.push_back("virtual ~" + class_name + "() = default;");
				lines.push_back({});

				// lines.push_back("public:");
				// lines.push_back(tab + "bool operator==(const " + class_name + "& other) const");
				// lines.push_back(tab + "{");
				// lines.push_back(tab + tab + "return impl == other.impl;");
				// lines.push_back(tab + "}");
				// lines.push_back({});
			}

			// void generate_cpp::write_member_func(const reflactor_structure& s)
			//{
			//	lines.push_back("public:");

			//	for (auto& mem : s.structs_)
			//	{
			//		if (mem.type_.empty())
			//			continue;

			//		auto type = get_type_name(mem.type_);

			//		if (type.empty())
			//			continue;

			//		lines.push_back(tab + type + " " + mem.name_ + "() const;" + crlf);
			//		lines.push_back(tab + "void set_" + mem.name_ + "(const " + type + "& " + mem.name_ + ");" + crlf);
			//		lines.push_back(tab + "void clear_" + mem.name_ + "();" + crlf);
			//	}
			//}

			void generate_cpp::write_internal_func_declare()
			{
				lines.push_back("private:");
				write_parse_func("from_binary");
				write_parse_func("to_binary");
			}

			void generate_cpp::write_parse_func(const std::string& func_name)
			{
				lines.push_back("virtual bool internal_" + func_name + "(elastic::flex_buffer_t& buffer) final;");
				lines.push_back({});
			}

			void generate_cpp::write_members(const std::vector<reflactor_structure>& rss)
			{
				lines.push_back("public:");

				for (auto& rs : rss)
				{
					lines.push_back(rs.type_ + " " + rs.name_ + ";" + rs.note_.content_);
				}
			}

			void generate_cpp::end_write_class()
			{
				lines.push_back("};");
			}

			void generate_cpp::real_write_file(std::ofstream& ofs)
			{
				std::string space{};

				std::string last_space{};

				for (auto& line : lines)
				{
					if (!line.empty())
					{
						if (line[0] == ')' || line[0] == '}')
							space.pop_back();
					}

					if (line.contains("public:") || line.contains("private:") || line.contains("protected:"))
					{
						ofs << last_space;
					}
					else
					{
						ofs << space;
					}

					ofs << line << crlf;

					if (line[0] == '{' || line[0] == '(')
					{
						last_space = space;
						space += tab;
					}
				}

				ofs.flush();
				ofs.close();
			}

			void generate_cpp::write_internal_func_def(const reflactor_structure& rs, const std::string& func_name)
			{
				lines.push_back("bool " + rs.name_ + "::internal_" + func_name + "(elastic::flex_buffer_t& buffer)");
				lines.push_back("{");

				for (auto& mem : rs.structs_)
				{
					if (mem.type_.empty())
						continue;

					auto type = get_type_name(mem.type_);

					if (type.empty())
						continue;

					lines.push_back("if (!elastic::" + func_name + "(" + mem.name_ + ",buffer))");
					lines.push_back("{");
					lines.push_back("return false;");
					lines.push_back("}");
					lines.push_back({});
				}

				lines.push_back("return true;");
				lines.push_back("}");
			}

		} // namespace cpp
	}	  // namespace compiler
} // namespace elastic