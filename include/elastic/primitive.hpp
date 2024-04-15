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
				primitive_guard(basic_primitive<value_type, traits_t>* primitive)
					: primitive_(primitive)
				{
					primitive_->start();
				}

				~primitive_guard()
				{
					primitive_->close();
				}

			private:
				basic_primitive<value_type, traits_t>* primitive_;
			};

			friend class primitive_guard;

		public:
			explicit basic_primitive(flex_buffer<value_type, traits_t>& bs)
				: streambuf_(bs)
			{}

			virtual ~basic_primitive() = default;

		public:
			void start()
			{
				streambuf_.start();
			}

			void close()
			{
				streambuf_.close();
			}

			void failed()
			{
				streambuf_.failed();
			}

		protected:
			flex_buffer<_Elem, _Traits>& streambuf_;
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

			virtual ~binary_iprimitive()
			{}

		public:
			bool load(value_type* data, const std::size_t size)
			{
				std::streamsize s = static_cast<std::streamsize>(size / sizeof(value_type));

				std::streamsize scount = this->streambuf_.load(data, size);

				return scount == s;
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

			virtual ~binary_oprimitive() = default;

		public:
			bool save(const value_type* data, const std::size_t size)
			{
				const auto result = this->streambuf_.save(data, size);

				return result == size;
			}
		};
	} // namespace detail
} // namespace elastic