#pragma once
#include <elastic/varint.hpp>

namespace elastic
{
	template <typename _Ty, typename _StreamBuf>
	struct message; 

	template <detail::sequence_t _Ty, typename _StreamBuf>
	struct sequence
	{
		static _Ty parse_binary(_StreamBuf& buf)
		{
			uint16_t bytes = varint<_StreamBuf>::template parse_binary<uint16_t>(buf);

			_Ty value{};

			for (uint16_t i = 0; i < bytes; ++i)
			{
				value.push_back(message<typename _Ty::value_type, _StreamBuf>::template parse_binary(buf));
			}

			return value;
		}

		static void to_binary(_Ty&& value, _StreamBuf& buf)
		{
			auto bytes = value.size();

			varint<_StreamBuf>::template to_binary(std::move(bytes), buf);

			for (auto s : std::forward<_Ty>(value))
			{
				message<typename std::remove_cvref_t<_Ty>::value_type, _StreamBuf>::template to_binary(std::move(s), buf);
			}
		}
	};
} // namespace elastic