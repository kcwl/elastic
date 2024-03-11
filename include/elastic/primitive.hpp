#pragma once
#include "flex_buffer.hpp"
#include <exception>
#include <iostream>
#include <string>

namespace elastic
{
	namespace detail
	{
		template <typename _Elem, typename _Traits>
		class basic_primitive
		{
		public:
			using element_t = _Elem;

			using traits_t = _Traits;

		private:
			class primitive_guard
			{
			public:
				primitive_guard(basic_primitive<element_t, traits_t>& primitive)
					: primitive_(primitive)
				{
					primitive_.start();
				}

				~primitive_guard()
				{
					primitive_.close();
				}

			private:
				basic_primitive<element_t, traits_t>& primitive_;
			};

			friend class primitive_guard;

		public:
			explicit basic_primitive(flex_buffer<element_t, traits_t>& bs)
				: streambuf_(bs)
				, start_pos_(0)
				, my_state_()
				, need_rollback_(false)
			{}

		public:
			template <typename _Func, typename... _Args>
			bool transcation(_Func&& f, _Args&&... Args)
			{
				primitive_guard lk(*this);

				try
				{
					std::forward<_Func>(f)(std::forward<_Args>(Args)...);
				}
				catch (...)
				{
					need_rollback_ = true;
				}

				return need_rollback_;
			}

			bool good()
			{
				return my_state_ & std::ios::goodbit;
			}

			bool fail()
			{
				return my_state_ & std::ios::failbit;
			}

		private:
			void complete()
			{
				my_state_ |= std::ios::goodbit;
			}

			void failed()
			{
				my_state_ &= ~std::ios::goodbit;

				my_state_ |= std::ios::failbit;
			}

			bool start()
			{
				if (start_pos_ != 0)
					return false;

				start_pos_ = static_cast<int32_t>(this->streambuf_.pubseekoff(0, std::ios::cur, std::ios::in));

				return true;
			}

			void close()
			{
				if (!need_rollback_)
				{
					this->complete();

					return;
				}

				this->streambuf_.pubseekpos(start_pos_, std::ios::in);

				start_pos_ = 0;

				this->failed();
			}

		protected:
			flex_buffer<_Elem, _Traits>& streambuf_;

		private:
			int32_t start_pos_;

			std::ios::iostate my_state_;

			bool need_rollback_;
		};

		template <typename _Archive, typename _Elem, typename _Traits = std::char_traits<_Elem>>
		class binary_iprimitive : public detail::basic_primitive<_Elem, _Traits>
		{
			using base_type = detail::basic_primitive<_Elem, _Traits>;

			using element_t = typename base_type::element_t;

			using traits_t = typename base_type::traits_t;

		protected:
			binary_iprimitive(flex_buffer<element_t, traits_t>& bs)
				: detail::basic_primitive<element_t, traits_t>(bs)
			{}

			~binary_iprimitive()
			{}

		public:
			template <pod_t _Ty>
			void load(_Ty& t)
			{
				constexpr auto array_size = sizeof(_Ty);

				element_t buffer[array_size] = { 0 };

				this->load(&buffer[0], array_size);

				t = *reinterpret_cast<_Ty*>(buffer);
			}

			void load(element_t* address, std::size_t size)
			{
				std::streamsize s = static_cast<std::streamsize>(size / sizeof(_Elem));

				std::streamsize scount = this->streambuf_.sgetn(static_cast<_Elem*>(address), s);

				if (scount != s)
				{
					throw std::underflow_error("input stream error!");
				}
			}
		};

		template <typename _Archive, typename _Elem, typename _Traits = std::char_traits<_Elem>>
		class binary_oprimitive : public detail::basic_primitive<_Elem, _Traits>
		{
			using base_type = detail::basic_primitive<_Elem, _Traits>;

			using element_t = typename base_type::element_t;

			using traits_t = typename base_type::traits_t;

		protected:
			binary_oprimitive(flex_buffer<element_t, traits_t>& bs)
				: detail::basic_primitive<element_t, traits_t>(bs)
			{}

			~binary_oprimitive() = default;

		public:
			template <pod_t _Ty>
			void save(_Ty&& t)
			{
				constexpr auto array_size = sizeof(_Ty);

				auto* elastic_fixed_ptr = reinterpret_cast<element_t*>(&t);

				this->save(elastic_fixed_ptr, array_size);
			}

			void save(element_t* begin, std::size_t size)
			{
				auto res = this->streambuf_.sputn(begin, size);

				if (res == 0)
				{
					throw std::overflow_error("output stream error!");
				}
			}
		};
	} // namespace detail
} // namespace elastic