#pragma once
#include <elastic/detail/concepts.hpp>

namespace elastic
{
	template <typename _Archive, typename _Elem, typename _Traits>
	class basic_binary_oprimitive
	{
	protected:
		basic_binary_oprimitive(std::basic_streambuf<_Elem, _Traits>& sb)
			: streambuf_(sb)
		{

		}

		~basic_binary_oprimitive() = default;

	public:
		template<typename _Ty>
		void save(_Ty&& t)
		{
			save_binary(std::addressof(t), sizeof(_Ty));
		}

		void save(const std::string& s)
		{
			auto l = s.size();

			save(l);

			save_binary(s.data(), l);
		}

		void save(const std::wstring& s)
		{
			auto l = s.size();

			save(l);

			save_binary(s.data(), sizeof(wchar_t) / sizeof(char) * l);
		}


		void save_binary(const void* address, std::size_t count)
		{
			count = (count + sizeof(_Elem) - 1) / sizeof(_Elem);
			std::streamsize scount =
				streambuf_.sputn(static_cast<const _Elem*>(address), static_cast<std::streamsize>(count));
			if (count != static_cast<std::size_t>(scount))
				throw(archive_exception(archive_exception::exception_code::output_stream_error));
		}

	protected:
		_Archive* _this()
		{
			return static_cast<_Archive*>(this);
		}

		void init()
		{
			this->_this()->save(static_cast<unsigned char>(sizeof(int)));
			this->_this()->save(static_cast<unsigned char>(sizeof(long)));
			this->_this()->save(static_cast<unsigned char>(sizeof(float)));
			this->_this()->save(static_cast<unsigned char>(sizeof(double)));
			// for checking endianness
			this->_this()->save(int(1));
		}

	protected:
		std::basic_streambuf<_Elem, _Traits>& streambuf_;
	};
} // namespace elastic