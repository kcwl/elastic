#pragma once
#include "flex_buffer.hpp"
#include "type_traits.hpp"
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
			using value_type = _Elem;

			using traits_t = _Traits;

		protected:
			class primitive_guard
			{
			public:
				primitive_guard(basic_primitive<value_type, traits_t>& primitive)
					: primitive_(primitive)
				{
					primitive_.start();
				}

				~primitive_guard()
				{
					primitive_.close();
				}

			private:
				basic_primitive<value_type, traits_t>& primitive_;
			};

			friend class primitive_guard;

		public:
			explicit basic_primitive(flex_buffer<value_type, traits_t>& bs)
				: streambuf_(bs)
				, start_pos_(0)
				, has_success_(true)
			{}

			virtual ~basic_primitive() = default;

		public:
			bool success() const
			{
				return has_success_;
			}

			void complete()
			{
				has_success_ = true;
			}

			void failed()
			{
				has_success_ = false;
			}

		private:
			bool start()
			{
				if (start_pos_ != 0)
					return false;

				start_pos_ = this->streambuf_.pubseekoff(0, std::ios::cur, std::ios::in);

				return true;
			}

			void close()
			{
				if (has_success_) [[likely]]
				{
					return;
				}

				this->streambuf_.pubseekpos(start_pos_, std::ios::in);

				start_pos_ = 0;
			}

		protected:
			flex_buffer<_Elem, _Traits>& streambuf_;

		private:
			int64_t start_pos_;

			bool has_success_;
		};

		template <typename _Archive, typename _Elem, typename _Traits = std::char_traits<_Elem>>
		class binary_iprimitive : public detail::basic_primitive<_Elem, _Traits>
		{
			using base_type = detail::basic_primitive<_Elem, _Traits>;

		public:
			using value_type = typename base_type::value_type;

			using traits_t = typename base_type::traits_t;

		protected:
			binary_iprimitive(flex_buffer<value_type, traits_t>& bs)
				: detail::basic_primitive<value_type, traits_t>(bs)
			{}

			~binary_iprimitive()
			{}

		public:
			template <pod_and_integer_t _Ty>
			void load(_Ty& t)
			{
				constexpr auto array_size = sizeof(_Ty);

				value_type buffer[array_size] = { 0 };

				this->load(&buffer[0], array_size);

				t = *reinterpret_cast<_Ty*>(buffer);
			}

			void load(value_type* address, std::size_t size)
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

		public:
			using value_type = typename base_type::value_type;

			using traits_t = typename base_type::traits_t;

		protected:
			binary_oprimitive(flex_buffer<value_type, traits_t>& bs)
				: detail::basic_primitive<value_type, traits_t>(bs)
			{}

			~binary_oprimitive() = default;

		public:
			template <pod_and_integer_t _Ty>
			void save(_Ty&& t)
			{
				constexpr auto array_size = sizeof(_Ty);

				auto* elastic_fixed_ptr = reinterpret_cast<value_type*>(&t);

				this->save(elastic_fixed_ptr, array_size);
			}

			void save(const value_type* begin, const std::size_t size)
			{
				this->streambuf_.sputn(begin, size);
			}
		};
	} // namespace detail
} // namespace elastic