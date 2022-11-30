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
        varint(_Ty value)
        {
            to_data(value);
        }

    public:
        template <detail::single_signed_numric _Ty>
        _Ty parse_data()
        {
            _Ty value = static_cast<_Ty>(read<uint8_t>());

            if constexpr (sizeof(_Ty) == 1)
            {
                return value;
            }
            else
            {
                value -= 0x80;

                int bit = 7;

                uint8_t c{};
                while (((c = read<uint8_t>()) & 0x80)!=0)
                {
                    value += c << bit;
                    value -= 0x80 << bit;

                    bit += 7;
                }

                value += static_cast<uint32_t>(c << bit);
            }

            return value;
        }

        template<detail::single_unsigned_numric _Ty>
        _Ty parse_data()
        {
        }

        template <detail::single_signed_numric _Ty>
        void to_data(_Ty&& value)
        {
            while (value > 0x80)
            {
                append(static_cast<uint8_t>(value | 0x80));
                value >>= 7;
            }

            append(static_cast<uint8_t>(value));
        }
    };
} // namespace elastic