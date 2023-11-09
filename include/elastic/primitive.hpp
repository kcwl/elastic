#pragma once
#include <exception>
#include <streambuf>
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
				, start_pos_(-1)
				, my_state_(0)
			{}

		public:
			bool transfer()
			{
				if (start_pos_ != -1)
					return false;

				start_pos_ = static_cast<int32_t>(this->streambuf_.pubseekoff(0, std::ios::cur, std::ios::in));

				return true;
			}

			void roll_back()
			{
				this->streambuf_.pubseekpos(start_pos_, std::ios::in);

				start_pos_ = -1;
			}

			void complete()
			{
				if (good())
					return;

				my_state_ = std::ios::iostate{};

				my_state_ |= std::ios::goodbit;
			}

			bool good()
			{
				return my_state_ & std::ios::goodbit;
			}

		protected:
			void fail()
			{
				my_state_ |= ~std::ios::goodbit;

				my_state_ |= std::ios::failbit;
			}

		protected:
			std::basic_streambuf<_Elem, _Traits>& streambuf_;

		private:
			int32_t start_pos_;

			std::ios::iostate my_state_;
		};
	} // namespace impl

	template <typename _Archive, typename _Elem, typename _Traits>
	class binary_iprimitive : public impl::basic_primitive<_Elem, _Traits>
	{
	protected:
		binary_iprimitive(std::basic_streambuf<_Elem, _Traits>& bs)
			: impl::basic_primitive<_Elem, _Traits>(bs)
		{}

		~binary_iprimitive()
		{}

	public:
		template <typename _Ty>
		void load(_Ty& t)
		{
			load_binary(&t, sizeof(_Ty));
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
			if (scount != 0)
				return;

			this->fail();

			throw std::exception("input stream error!");
		}
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