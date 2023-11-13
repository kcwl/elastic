#pragma once
#include "flex_buffer.hpp"

#include <exception>
#include <iostream>

namespace elastic
{
	namespace impl
	{
		template <typename _Elem, typename _Traits>
		class basic_primitive
		{
		public:
			explicit basic_primitive(flex_buffer<_Elem, _Traits>& bs)
				: streambuf_(bs)
				, start_pos_(0)
				, my_state_(0)
			{}

		public:
			bool transfer()
			{
				if (start_pos_ != 0)
					return false;

				start_pos_ = static_cast<int32_t>(this->streambuf_.pubseekoff(0, std::ios::cur, std::ios::in));

				return true;
			}

			void roll_back()
			{
				this->streambuf_.pubseekpos(start_pos_, std::ios::in);

				start_pos_ = 0;
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
				return (my_state_ & std::ios::goodbit) == std::ios::goodbit;
			}

		protected:
			void fail()
			{
				my_state_ |= ~std::ios::goodbit;

				my_state_ |= std::ios::failbit;
			}

		protected:
			flex_buffer<_Elem, _Traits>& streambuf_;

		private:
			int32_t start_pos_;

			std::ios::iostate my_state_;
		};
	} // namespace impl

	template <typename _Archive, typename _Elem, typename _Traits>
	class binary_iprimitive : public impl::basic_primitive<_Elem, _Traits>
	{
	protected:
		binary_iprimitive(flex_buffer<_Elem, _Traits>& bs)
			: impl::basic_primitive<_Elem, _Traits>(bs)
		{}

		~binary_iprimitive()
		{}

	public:
		template <typename _Ty>
		void load(_Ty& t)
		{
			constexpr auto array_size = sizeof(_Ty);

			_Elem buffer[array_size] = { 0 };

			this->load(&buffer[0], array_size);

			t = *reinterpret_cast<_Ty*>(buffer);
		}

		template <typename _Ty>
		void load(_Ty* address, std::size_t size)
		{
			std::streamsize s = static_cast<std::streamsize>(size / sizeof(_Elem));

			std::streamsize scount = this->streambuf_.sgetn(static_cast<_Elem*>(address), s);

			if (scount != 0)
			{
				this->complete();

				return;
			}

			this->fail();

			throw std::exception("input stream error!");
		}

		void get(uint8_t& c)
		{
			c = *this->streambuf_.wdata();

			this->streambuf_.consume(1);
		}

	protected:
		_Archive* _this()
		{
			return static_cast<_Archive*>(this);
		}
	};

	template <typename _Archive, typename _Elem, typename _Traits>
	class binary_oprimitive : public impl::basic_primitive<_Elem, _Traits>
	{
	protected:
		binary_oprimitive(flex_buffer<_Elem, _Traits>& bs)
			: impl::basic_primitive<_Elem, _Traits>(bs)
		{}

		~binary_oprimitive() = default;

	public:
		template <typename _Ty>
		void save(_Ty&& t)
		{
			constexpr auto array_size = sizeof(_Ty);

			using type = std::remove_cvref_t<_Ty>;

			union 
			{
				type t;
				char c[array_size];
			} elastic_fixed{};

			elastic_fixed.t = std::forward<_Ty>(t);

			for (auto& m : elastic_fixed.c)
			{
				put(m);
			}
		}

		template <typename _Ty>
		void save(_Ty* begin, std::size_t size)
		{
			for (std::size_t i = 0; i < size; ++i)
			{
				put(begin[i]);
			}
		}

		void put(uint8_t c)
		{
			*this->streambuf_.rdata() = c;

			this->streambuf_.commit(1);
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