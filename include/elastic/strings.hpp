#pragma once
#include <elastic/detail/basic_streambuf.hpp>

namespace elastic
{
    class strings : public detail::basic_streambuf<uint8_t>
    {
    public:
        strings()
            : basic_streambuf()
        {
        }

        template <typename _Iter>
        strings(_Iter begin, _Iter end)
            : basic_streambuf(begin, end)
        {
        }

        template<typename _Ty>
        strings(_Ty&& value)
        {
            to_data(std::forward<_Ty>(value));
        }

    public:
        template <typename _Ty>
        _Ty parse_data()
        {
        }

        template<typename _Ty>
        void to_data(_Ty&& value)
        {
            value;
        }
    };
} // namespace elastic