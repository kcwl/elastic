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
        template <typename _Ty>
        _Ty parse_data()
        {
            return _Ty{};
        }

        template <typename _Ty>
        void to_data(_Ty&& value)
        {
            if constexpr (detail::single_numric<std::remove_cvref_t<_Ty>>)
            {
                if (value < 0x80)
                {
                    length_ = 1;
                    
                }
                else if (value< 0x8000)
                {
                    length_ = 2;
                }
                else if (value<0x800000)
                {
                    length_ = 3;
                }
                else if (value < 0x80000000)
                {
                    length_ = 4;
                }

                append(std::forward<_Ty>(value), length_);
            }
        }

    private:
        int32_t length_;
    };
} // namespace elastic