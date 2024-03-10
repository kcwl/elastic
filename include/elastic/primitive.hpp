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
				, my_state_()
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
				my_state_ |= std::ios::goodbit;
			}

			bool good()
			{
				return my_state_ & std::ios::goodbit;
			}

			bool fail()
			{
				return my_state_ & std::ios::failbit;
			}

		protected:
			void failed()
			{
				my_state_ &= ~std::ios::goodbit;

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
		using element_type = _Elem;

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

			element_type buffer[array_size] = { 0 };

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

			this->failed();

			throw std::runtime_error("input stream error!");
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
		using element_type = _Elem;

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

			auto* elastic_fixed_ptr = reinterpret_cast<element_type*>(&t);

			this->save(elastic_fixed_ptr, array_size);
		}

		template <typename _Ty>
		void save(_Ty* begin, std::size_t size)
		{
			auto res = this->streambuf_.sputn(begin, size);

			if (res != 0)
			{
				this->complete();

				return;
			}

			this->failed();

			throw std::runtime_error("output stream error!");
		}

	protected:
		_Archive* _this()
		{
			return static_cast<_Archive*>(this);
		}

	private:
		void save_binary(const element_type* address, std::size_t count)
		{
			count = (count + sizeof(element_type) - 1) / sizeof(element_type);

			this->streambuf_.sputn(address, count);
		}
	};
} // namespace elastic