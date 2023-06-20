#pragma once
#include "archive_exception.hpp"
#include "basic_primitive.hpp"

namespace elastic
{
	template <typename _Archive, typename _Elem, typename _Traits>
	class basic_iprimitive : public basic_primitive<_Elem, _Traits>
	{
	protected:
		basic_iprimitive(std::basic_streambuf<_Elem, _Traits>& bs)
			: basic_primitive<_Elem, _Traits>(bs)
			, trans_pos_(0)
			, interrupt_(false)
		{}

		~basic_iprimitive()
		{}

	public:
		template <typename _Ty>
		void load(_Ty& t)
		{
			load_binary(&t, sizeof(_Ty));
		}

		template <>
		void load(std::string& s)
		{
			std::size_t l = this->_this()->load<std::size_t>();

			s.resize(l);

			if (0 < l)
				load_binary(&(*s.begin()), l);
		}

		template <>
		void load(std::wstring& ws)
		{
			std::size_t l{};

			this->_this()->load(l);

			ws.resize(l);

			load_binary(const_cast<wchar_t*>(ws.data()), l * sizeof(wchar_t) / sizeof(char));
		}

		void load_binary(void* address, std::size_t count)
		{
			std::streamsize s = static_cast<std::streamsize>(count / sizeof(_Elem));
			std::streamsize scount = this->streambuf_.sgetn(static_cast<_Elem*>(address), s);
			if (scount == 0)
				throw(archive_exception(archive_exception::exception_number::input_stream_error));
		}

		void start()
		{
			if (trans_pos_ != 0)
				return;

			trans_pos_ = static_cast<int32_t>(this->streambuf_.pubseekoff(0, std::ios::cur, std::ios::in));
		}

		void roll_back()
		{
			this->streambuf_.pubseekpos(trans_pos_, std::ios::in);

			trans_pos_ = 0;

			interrupt(true);
		}

		void interrupt(bool f)
		{
			interrupt_ = f;
		}

		bool interrupt()
		{
			return interrupt_;
		}

	protected:
		_Archive* _this()
		{
			return static_cast<_Archive*>(this);
		}

	protected:
		int32_t trans_pos_;

		bool interrupt_;
	};
} // namespace elastic