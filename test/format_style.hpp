#pragma once
#include <elastic/detail/concepts.hpp>

namespace test
{
    enum class color : int8_t
    {
        red,
        yellow,
        blue
    };

    template <typename _Ty>
    requires(std::is_same_v<_Ty, int>)
    class int_format
    {
    public:
        void switch_format()
        {
            switch (color)
            {
            case test::color::red:
                {
                }
                break;
            case test::color::yellow:
                break;
            case test::color::blue:
                break;
            default:
                break;
            }
        }

    public:
        int a_;
        int b_;
        int* ptr_;
    };
} // namespace test