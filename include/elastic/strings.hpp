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
    };
} // namespace elastic