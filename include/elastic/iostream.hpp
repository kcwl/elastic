#pragma once
#include <elastic/detail/basic_streambuf.hpp>
#include <elastic/message.hpp>
#include <elastic/attribute.hpp>

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
		{}

		iostream(const message_buffer& ios)
			: buffer_(ios)
		{}

		template <typename _It>
		iostream(_It begin, _It end)
			: buffer_(begin, end)
		{}

		template <typename _Ty, std::size_t N>
		iostream(std::span<_Ty, N> buf)
			: buffer_(buf)
		{}

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

		template <detail::sequence_t _Ty>
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
			message<_Ty, message_buffer>::template to_binary(std::forward<_Ty>(value), buffer_);
		}
		
		template<detail::sequence_t _Ty>
		void push(_Ty&& value)
		{
			sequence<_Ty, message_buffer>::template to_binary(std::move(value), buffer_);
		}

		template<attribute _Ty>
		void push(_Ty&& value)
		{
			if constexpr (optional_t<std::remove_cvref_t<_Ty>>)
			{
				if constexpr (_Ty::require_value)
				{
					if (!value.has_value())
					{
						throw std::runtime_error("maybe some type must have some values!");
					}
					else
					{
						push(*value);
					}
					
				}
				else
				{
					push(*value);
				}
			}
			else
			{
				buffer_.append(std::move(value).value_);
			}
		}


		template <std::size_t I, typename _Ty>
		auto make_element()
		{
			auto element = elastic::get<I>(_Ty{});

			using element_t = decltype(element);

			if constexpr (detail::varint<element_t>)
			{
				return varint<message_buffer>::template parse_binary<element_t>(buffer_);
			}
			else if constexpr (detail::string_t<element_t>)
			{
				return sequence<element_t, message_buffer>::template parse_binary(buffer_);
			}
			else
			{
				return pop<element_t>();
			}
		}

		template <typename _Ty, std::size_t... I>
		auto pop_element(std::index_sequence<I...>)
		{
			return _Ty{ make_element<I, _Ty>()... };
		}

		template <detail::sequence_t _Ty>
		_Ty pop_string()
		{
			return sequence<_Ty, message_buffer>::template parse_binary(buffer_);
		}

		template <typename _Ty, std::size_t N = elastic::tuple_size_v<_Ty>,
				  typename Indices = std::make_index_sequence<N>>
		_Ty pop()
		{
			return pop_element<_Ty>(Indices{});
		}

		template<optional_t _Ty>
		_Ty pop()
		{
			using type = typename _Ty::value_type;

			type val = pop<type>();

			_Ty value{};

			value.emplace(val);

			return value;
		}

		template<fixed_t _Ty>
		_Ty pop()
		{
			using type = typename _Ty::type;
			
			_Ty value{};

			buffer_.read(&value.value_, sizeof(type));

			return value;
		}

		template<unsign_t _Ty>
		_Ty pop()
		{
			using type = typename _Ty::value_type;

			_Ty value{};

			buffer_.read(&value.value_, sizeof(type));

			return value;
		}

	private:
		message_buffer buffer_;
	};
} // namespace elastic