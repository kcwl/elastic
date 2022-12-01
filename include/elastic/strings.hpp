#pragma once
#include <elastic/varint.hpp>

namespace elastic
{
	template <detail::string _Ty,typename _StreamBuf>
	struct strings
	{
		static _Ty parse_binary(_StreamBuf& buf)
		{
			uint16_t bytes = varint<_StreamBuf>::parse_binary<uint16_t>(buf);

			_Ty value{};

			for (uint16_t i = 0; i < bytes; ++i)
			{
				value.push_back(static_cast<typename _Ty::value_type>(buf.read<uint8_t>()));
			}

			return value;
		}

		static void to_binary(_Ty&& value, _StreamBuf& buf)
		{
			auto bytes = value.size();

			varint<_StreamBuf>::to_binary(std::move(bytes), buf);

			for (auto& s : value)
			{
				buf.append(static_cast<typename _StreamBuf::value_type>(s));
			}
		}
	};
} // namespace elastic