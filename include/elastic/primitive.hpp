#pragma once
#include "exception.hpp"
#include <iostream>

namespace elastic
{
	namespace impl
	{
		template <typename _Elem, typename _Traits>
		class basic_primitive
		{
		public:
			explicit basic_primitive(std::basic_streambuf<_Elem, _Traits>& bs)
				: streambuf_(bs)
			{}

		protected:
			std::basic_streambuf<_Elem, _Traits>& streambuf_;
		};
	}

	template <typename _Archive, typename _Elem, typename _Traits>
	class binary_iprimitive : public impl::basic_primitive<_Elem, _Traits>
	{
	protected:
		binary_iprimitive(std::basic_streambuf<_Elem, _Traits>& bs)
			: impl::basic_primitive<_Elem, _Traits>(bs)
			, trans_pos_(0)
			, interrupt_(false)
		{}

		~binary_iprimitive()
		{}

	public:
		template <typename _Ty>
		void load(_Ty& t)
		{
			load_binary(&t, sizeof(_Ty));
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

	private:
		void load_binary(void* address, std::size_t count)
		{
			std::streamsize s = static_cast<std::streamsize>(count / sizeof(_Elem));
			std::streamsize scount = this->streambuf_.sgetn(static_cast<_Elem*>(address), s);
			if (scount == 0)
				throw(archive_exception(archive_exception::exception_number::input_stream_error));
		}

	protected:
		int32_t trans_pos_;

		bool interrupt_;
	};

	template <typename _Archive, typename _Elem, typename _Traits>
	class binary_oprimitive : public impl::basic_primitive<_Elem, _Traits>
	{
	protected:
		binary_oprimitive(std::basic_streambuf<_Elem, _Traits>& bs)
			: impl::basic_primitive<_Elem, _Traits>(bs)
		{}

		~binary_oprimitive() = default;

	public:
		template <typename _Ty>
		void save(_Ty&& t)
		{
			save_binary(std::addressof(t), sizeof(_Ty));
		}

	protected:
		_Archive* _this()
		{
			return static_cast<_Archive*>(this);
		}

	private:
		void save_binary(const void* address, std::size_t count)
		{
			count = (count + sizeof(_Elem) - 1) / sizeof(_Elem);

			this->streambuf_.sputn(static_cast<const _Elem*>(address), static_cast<std::streamsize>(count));
		}
	};
} // namespace elastic