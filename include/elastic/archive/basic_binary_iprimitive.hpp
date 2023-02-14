#pragma once
#include <elastic/detail/concepts.hpp>
#include <streambuf>
#include <elastic/archive/archive_exception.hpp>

namespace elastic
{
	template<typename _Archive, typename _Elem, typename _Traits>
	class basic_binary_iprimitive
	{
	protected:
		basic_binary_iprimitive(std::basic_streambuf<_Elem, _Traits>& sb)
			: buffer_(sb)
		{

		}

		~basic_binary_iprimitive()
		{
		}

	public:
		struct use_array_optimization
		{
			template<typename _Ty>
			struct apply : std::is_arithmetic<_Ty>
			{};
		};

		template<typename _Ty>
		_Ty load()
		{
			_Ty t{};

			load_binary(&t, sizeof(_Ty));

			return t;
		}

		template<>
		std::string load<std::string>()
		{
			std::string s{};

			std::size_t l = this->_this()->load<std::size_t>();

			s.resize(l);

			if (0 < l)
				load_binary(&(*s.begin()), l);
		}

		template<>
		std::wstring load<std::wstring>()
		{
			std::wstring ws{};

			std::size_t l = this->_this()->load();

			ws.resize(l);

			load_binary(const_cast<wchar_t*>(ws.data()), l * sizeof(wchar_t) / sizeof(char));
		}

		void load_binary(void* address, std::size_t count)
		{
			std::streamsize s = static_cast<std::streamsize>(count / sizeof(_Elem));
			std::streamsize scount = buffer_.sgetn(static_cast<_Elem*>(address), s);
			if (scount != s)
				throw(archive_exception(archive_exception::exception_code::input_stream_error));

			s = static_cast<std::streamsize>(count % sizeof(_Elem));
			if (0 < s)
			{
				_Elem t{};
				scount = buffer_.sgetn(&t, 1);
				if (scount != 1)
					throw(archive_exception(archive_exception::exception_code::input_stream_error));
				std::memcpy(static_cast<char*>(address) + (count - s), &t, static_cast<std::size_t>(s));
			}
		}

		void init()
		{
			unsigned char size{};

			this->_this()->load(size);
			if (size != sizeof(int))
			{
				throw(archive_exception(archive_exception::exception_code::incompatible_native_format, "size of int"));
			}

			this->_this()->load(size);
			if (size != sizeof(long))
			{
				throw(archive_exception(archive_exception::exception_code::incompatible_native_format, "size of long"));
			}

			this->_this()->load(size);
			if (size != sizeof(float))
			{
				throw(archive_exception(archive_exception::exception_code::incompatible_native_format, "size of float"));
			}

			this->_this()->load(size);
			if (size != sizeof(double))
			{
				throw(archive_exception(archive_exception::exception_code::incompatible_native_format, "size of double"));
			}
		}

	protected:
		_Archive* _this()
		{
			return static_cast<_Archive*>(this);
		}

	protected:
		std::basic_streambuf<_Elem, _Traits>& buffer_;
	};
}