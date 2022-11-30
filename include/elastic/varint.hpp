#pragma once
#include <elastic/detail/basic_streambuf.hpp>

namespace elastic
{
	class varint : public detail::basic_streambuf<uint8_t>
	{
	public:
		varint()
			: basic_streambuf(128)
		{
		}

		template <typename _Iter>
		varint(_Iter begin, _Iter end)
			: basic_streambuf(begin, end)
		{
		}

		template <typename _Ty>
		varint(_Ty&& value)
		{
			to_data(std::move(value));
		}

	public:
		template <detail::single_numric _Ty>
		_Ty parse_data()
		{
			uint64_t value = read<uint8_t>();

			if constexpr (sizeof(_Ty) == 1)
			{
				return value;
			}
			else
			{
				if (value > 0x80)
				{
					value -= 0x80;

					int bit = 7;

					uint8_t c{};
					while (((c = read<uint8_t>()) & 0x80) != 0)
					{
						value += c << bit;
						value -= 0x80 << bit;

						bit += 7;
					}

					value += static_cast<uint32_t>(c << bit);
				}
			}

			value % 2 == 0 ? value /= 2 : value = (0ul-(value - 1)) / 2;

			return static_cast<_Ty>(value);
		}

		template <detail::multi_numric _Ty>
		_Ty parse_data()
		{
			return read<_Ty>();
		}

		template <detail::single_numric _Ty>
		void to_data(_Ty&& value)
		{
			uint64_t result{};
			value < 0 ? result = (0 - value)* 2 + 1 : result = value * 2;

			while (result > 0x80)
			{
				append(static_cast<uint8_t>(result | 0x80));
				result >>= 7;
			}

			append(static_cast<uint8_t>(result));
		}

		template <detail::multi_numric _Ty>
		void to_data(_Ty&& value)
		{
			append(std::forward<_Ty>(value));
		}
	};
} // namespace elastic