#pragma once
#include <elastic/varint.hpp>

namespace elastic
{
	template <detail::string_t _Ty,typename _StreamBuf>
	struct strings
	{
		static _Ty parse_binary(_StreamBuf& buf)
		{
			uint16_t bytes = varint<_StreamBuf>::template parse_binary<uint16_t>(buf);

			_Ty value{};

			for (uint16_t i = 0; i < bytes; ++i)
			{
				value.push_back(varint<_StreamBuf>::template parse_binary<typename _Ty::value_type>(buf));
			}

			return value;
		}

		static void to_binary(_Ty&& value, _StreamBuf& buf)
		{
			auto bytes = value.size();

			varint<_StreamBuf>::to_binary(std::move(bytes), buf);

			for (auto s : std::forward<_Ty>(value))
			{
				varint<_StreamBuf>::to_binary(std::move(s), buf);
			}
		}
	};
} // namespace elastic