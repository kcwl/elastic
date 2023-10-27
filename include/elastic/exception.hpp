#pragma once
#include <algorithm>
#include <exception>

namespace elastic
{
	class archive_exception : public virtual std::exception
	{
	public:
		enum class exception_number
		{
			input_stream_error,
			output_stream_error
		};

	public:
		archive_exception(exception_number c, const char* e1 = nullptr, const char* e2 = nullptr) noexcept
			: code_(c)
			, buffer_()
		{
			unsigned int length = 0;
			switch (code_)
			{
			case exception_number::input_stream_error:
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
			case exception_number::output_stream_error:
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
			while (l < (sizeof(buffer_) - 1))
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
		exception_number code_;

	private:
		char buffer_[128];
	};
} // namespace elastic