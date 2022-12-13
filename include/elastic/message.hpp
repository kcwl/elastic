#pragma once
#include <elastic/strings.hpp>
#include <elastic/tuple_size.hpp>
#include <elastic/reflect.hpp>

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
		static _Ty parse_binary(_StreamBuf& buf)
		{
			_Ty value{};

			for_each(std::move(value),
					 [this](auto& v)
					 {
						 using type = decltype(v);
						 if constexpr (detail::varint<std::remove_cvref_t<type>>)
						 {
							 v = varint<_StreamBuf>::template parse_binary<type>(buf);
						 }
						 else if constexpr (detail::string_t<std::_Remove_cvref_t<type>>)
						 {
							 v = strings<type, _StreamBuf>::template parse_binary(buf);
						 }
						 else
						 {
							 v = message<type, _StreamBuf>::template parse_binary(buf);
						 }
					 });

			return value;
		}

		static void to_binary(_Ty&& value, _StreamBuf& buf)
		{
			// auto bytes = value.size();

			// varint<_StreamBuf>::to_binary(std::move(bytes), buf);

			// for (auto s : std::forward<_Ty>(value))
			//{
			//	varint<_StreamBuf>::to_binary(std::move(s), buf);
			// }
			for_each(std::move(value),
					 [&](auto&& v)
					 {
						 using type = decltype(v);
						 if constexpr (detail::varint<std::remove_cvref_t<type>>)
						 {
							 varint<_StreamBuf>::template to_binary(std::move(v), buf);
						 }
						 else if constexpr (detail::string_t<std::_Remove_cvref_t<type>>)
						 {
							 strings<type, _StreamBuf>::template to_binary(std::move(v), buf);
						 }
						 else
						 {
							 message<type, _StreamBuf>::to_binary(std::move(v), buf);
						 }
					 });
		}
	};
} // namespace elastic