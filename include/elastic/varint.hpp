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

    private:
        detail::basic_streambuf<uint8_t> buf_;
    };
} // namespace elastic