#pragma once
#include <elastic/detail/basic_streambuf.hpp>
#include <elastic/reflect.hpp>
#include <elastic/strings.hpp>
#include <elastic/tuple_size.hpp>

namespace
{
	template <typename _Ty, typename Func, std::size_t... I>
	constexpr void for_each(_Ty&& val, Func func, std::index_sequence<I...>)
	{
		return (func(elastic::get<I>(std::forward<_Ty>(val))), ...);
	}

	template <typename _Ty, typename Func, std::size_t N = elastic::tuple_size_v<_Ty>,
			  typename Indices = std::make_index_sequence<N>>
	constexpr void for_each(_Ty&& val, Func func)
	{
		return for_each(std::forward<_Ty>(val), func, Indices{});
	}
} // namespace

namespace elastic
{
	using message_buffer = detail::basic_streambuf<uint8_t>;

	class iostream
	{
		using message_buffer_value_type = message_buffer::value_type;

		using pointer = typename message_buffer::pointer;

		using const_pointer = typename message_buffer::const_pointer;

	public:
		iostream()
			: buffer_()
		{
		}

		iostream(const message_buffer& ios)
			: buffer_(ios)
		{
		}

		iostream(std::span<message_buffer_value_type> data)
			: buffer_(data)
		{
		}

	public:
		template <typename _Ty>
		iostream& operator<<(_Ty&& value)
		{
			push(std::move(value));
			return *this;
		}

		template <typename _Ty>
		iostream& operator>>(_Ty& value)
		{
			value = pop<_Ty>();
			return *this;
		}

		template<detail::string_t _Ty>
		iostream& operator>>(_Ty& value)
		{
			value = pop_string<_Ty>();
			return *this;
		}

		void clear()
		{
			return buffer_.clear();
		}

		message_buffer::pointer data() noexcept
		{
			return buffer_.data();
		}

		const_pointer data() const noexcept
		{
			return buffer_.data();
		}

		pointer wdata() noexcept
		{
			return buffer_.wdata();
		}

		const_pointer wdata() const noexcept
		{
			return buffer_.wdata();
		}

		auto rdata() noexcept
		{
			return buffer_.rdata();
		}

		const auto rdata() const noexcept
		{
			return buffer_.rdata();
		}

		std::size_t size() noexcept
		{
			return buffer_.size();
		}

		const std::size_t size() const noexcept
		{
			return buffer_.size();
		}

		void commit(int n) noexcept
		{
			return buffer_.commit(n);
		}

		void consume(int length)
		{
			return buffer_.consume(length);
		}

	private:
		template <typename _Ty>
		void push(_Ty&& value)
		{
			for_each(std::move(value),
					 [this](auto&& v)
					 {
						 if constexpr (detail::varint<std::remove_cvref_t<decltype(v)>>)
						 {
							 varint<message_buffer>::template to_binary(std::move(v), buffer_);
						 }
						 else
						 {
							 strings<decltype(v), message_buffer>::template to_binary(std::move(v), buffer_);
						 }
					 });
		}

		template <detail::string_t _Ty>
		void push(_Ty&& value)
		{
			strings<_Ty, message_buffer>::template to_binary(std::move(value), buffer_);
		}

		template <std::size_t I, typename _Ty>
		auto make_element()
		{
			auto element = elastic::get<I>(_Ty{});

			using element_t = decltype(element);

			if constexpr (detail::varint<element_t>)
			{
				return varint<message_buffer>::template parse_binary<element_t, message_buffer>(buffer_);
			}
			else if constexpr (detail::string_t<element_t>)
			{
				return strings<element_t, message_buffer>::template parse_binary(buffer_);
			}
			else
			{
				return pop<element_t>();
			}
		}

		template <typename _Ty, std::size_t... I>
		auto pop_element(std::index_sequence<I...>)
		{
			return _Ty{make_element<I, _Ty>()...};
		}

		template <detail::string_t _Ty>
		_Ty pop_string()
		{
			return strings<_Ty, message_buffer>::template parse_binary(buffer_);
		}

		template <typename _Ty, std::size_t N = elastic::tuple_size_v<_Ty>,
				  typename Indices = std::make_index_sequence<N>>
		_Ty pop()
		{
			return pop_element<_Ty>(Indices{});
		}

	private:
		message_buffer buffer_;
	};
} // namespace elastic