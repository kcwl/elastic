#pragma once
#include <elastic/reflect.hpp>
#include <elastic/sequence.hpp>

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
	template <typename _Ty, typename _StreamBuf>
	struct message
	{
		template <std::size_t I, typename _Ty>
		static auto make_element(_StreamBuf& buf)
		{
			auto element = elastic::get<I>(_Ty{});

			using element_t = decltype(element);

			if constexpr (detail::varint<element_t>)
			{
				return varint<_StreamBuf>::template parse_binary<element_t>(buf);
			}
			else if constexpr (detail::sequence_t<element_t>)
			{
				return sequence<element_t, _StreamBuf>::template parse_binary(buf);
			}
			else
			{
				return message<element_t, _StreamBuf>::template parse_binary(buf);
			}
		}

		template <typename _Ty, std::size_t... I>
		static auto pop_element(_StreamBuf& buf, std::index_sequence<I...>)
		{
			return _Ty{ make_element<I, _Ty>(buf)... };
		}

		static _Ty parse_binary(_StreamBuf& buf)
		{
			constexpr auto N = elastic::tuple_size_v<_Ty>;

			using Indices = std::make_index_sequence<N>;

			_Ty value = pop_element<_Ty>(buf, Indices{});

			return value;
		}

		static void to_binary(_Ty&& value, _StreamBuf& buf)
		{
			for_each(std::move(value),
					 [&](auto&& v)
					 {
						 using type = decltype(v);
						 if constexpr (detail::varint<std::remove_cvref_t<type>>)
						 {
							 varint<_StreamBuf>::template to_binary(std::move(v), buf);
						 }
						 else if constexpr (detail::sequence_t<std::_Remove_cvref_t<type>>)
						 {
							 sequence<type, _StreamBuf>::template to_binary(std::move(v), buf);
						 }
						 else
						 {
							 message<type, _StreamBuf>::to_binary(std::move(v), buf);
						 }
					 });
		}
	};
} // namespace elastic