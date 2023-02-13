#pragma once
#include <exception>
#include <algorithm>

namespace elastic
{
	class archive_exception : public virtual std::exception
	{
	public:
		enum class exception_code
		{
			no_exception,
			other_exception,
			unregistered_class,
			invalid_signature,
			unsupported_version,
			pointer_conflict,
			incompatible_native_format,
			array_size_too_short,
			input_stream_error,
			invalid_class_name,
			unregistered_cast,
			unsupported_class_version,
			multiple_code_instantiation,
			output_stream_error
		};

	public:
		archive_exception(exception_code c, const char* e1 = nullptr, const char* e2 = nullptr) noexcept
			: code_(c)
			, buffer_()
		{
			unsigned int length = 0;
			switch (code_)
			{
			case exception_code::no_exception:
				length = append(length, "uninitialized exception");
				break;
			case exception_code::unregistered_class:
				length = append(length, "unregistered class");
				if (NULL != e1)
				{
					length = append(length, " - ");
					length = append(length, e1);
				}
				break;
			case exception_code::invalid_signature:
				length = append(length, "invalid signature");
				break;
			case exception_code::unsupported_version:
				length = append(length, "unsupported version");
				break;
			case exception_code::pointer_conflict:
				length = append(length, "pointer conflict");
				break;
			case exception_code::incompatible_native_format:
				length = append(length, "incompatible native format");
				if (NULL != e1)
				{
					length = append(length, " - ");
					length = append(length, e1);
				}
				break;
			case exception_code::array_size_too_short:
				length = append(length, "array size too short");
				break;
			case exception_code::input_stream_error:
				length = append(length, "input stream error");
				if (NULL != e1)
				{
					length = append(length, "-");
					length = append(length, e1);
				}
				if (NULL != e2)
				{
					length = append(length, "-");
					length = append(length, e2);
				}
				break;
			case exception_code::invalid_class_name:
				length = append(length, "class name too long");
				break;
			case exception_code::unregistered_cast:
				length = append(length, "unregistered void cast ");
				length = append(length, (NULL != e1) ? e1 : "?");
				length = append(length, "<-");
				length = append(length, (NULL != e2) ? e2 : "?");
				break;
			case exception_code::unsupported_class_version:
				length = append(length, "class version ");
				length = append(length, (NULL != e1) ? e1 : "<unknown class>");
				break;
			case exception_code::other_exception:
				// if get here - it indicates a derived exception
				// was sliced by passing by value in catch
				length = append(length, "unknown derived exception");
				break;
			case exception_code::multiple_code_instantiation:
				length = append(length, "code instantiated in more than one module");
				if (NULL != e1)
				{
					length = append(length, " - ");
					length = append(length, e1);
				}
				break;
			case exception_code::output_stream_error:
				length = append(length, "output stream error");
				if (NULL != e1)
				{
					length = append(length, "-");
					length = append(length, e1);
				}
				if (NULL != e2)
				{
					length = append(length, "-");
					length = append(length, e2);
				}
				break;
			default:
				length = append(length, "programming error");
				break;
			}
		}

	protected:
		archive_exception() = default;

	public:
		archive_exception(const archive_exception& oth) noexcept
			: std::exception(oth)
			, code_(oth.code_)
		{
			std::memcpy(buffer_, oth.buffer_, sizeof(buffer_));
		}

		~archive_exception() noexcept override = default;

		const char* what() const noexcept override
		{
			return buffer_;
		}
		

	protected:
		uint32_t append(uint32_t l, const char* a)
		{
			while(l < (sizeof(buffer_) - 1))
			{
				char c = *a++;
				if ('\0' == c)
					break;
				buffer_[l++] = c;
			}
			buffer_[l] = '\0';
			return l;
		}

	public:
		exception_code code_;

	private:
		char buffer_[128];
	};
}