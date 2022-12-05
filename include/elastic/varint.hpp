#pragma once
#include <array>
#include <elastic/detail/basic_streambuf.hpp>
#include <elastic/int128.hpp>

namespace elastic
{
	template <typename _StreamBuf>
	struct varint
	{
		template <detail::single_numric _Ty>
		static _Ty parse_binary(_StreamBuf& buf)
		{
			uint128_t value = buf.read<uint8_t>();

			if (value > 0x80)
			{
				value -= 0x80;

				uint8_t bit = 7;

				uint8_t c{};
				while (((c = buf.read<uint8_t>()) & 0x80) != 0)
				{
					value += static_cast<uint128_t>(c) << bit;
					value -= static_cast<uint128_t>(0x80) << bit;

					bit += 7;
				}

				value += static_cast<uint128_t>(c) << bit;
			}

			value % 2 == 0 ? value /= 2 : value = (uint128_t{ 0 } - (value - 1)) / 2;

			return static_cast<_Ty>(value.low());
		}

		template <detail::multi_numric _Ty>
		static _Ty parse_binary(_StreamBuf& buf)
		{
			return buf.read<_Ty>();
		}

		template <detail::single_numric _Ty>
		static void to_binary(_Ty&& value, _StreamBuf& buf)
		{
			uint128_t result{};
			value < 0 ? result = (0 - uint128_t{ 0 - value })* 2 + 1 : result = uint128_t{ value } * 2;

			while (result > 0x80)
			{
				buf.append(static_cast<uint8_t>(result | 0x80));
				result >>= 7;
			}

			buf.append(static_cast<uint8_t>(result));
		}

		template <detail::multi_numric _Ty>
		static void to_binary(_Ty&& value, _StreamBuf& buf)
		{
			buf.append(std::forward<_Ty>(value));
		}
	};
} // namespace elastic