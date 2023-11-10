#pragma once
#include "elastic/access.hpp"
#include "elastic/archive.hpp"
#include "elastic/flex_buffer.hpp"
#include "elastic/message_lite.hpp"
#include "elastic/reflect.hpp"

using fixed32_t = elastic::fixed<uint32_t>;
using fixed64_t = elastic::fixed<uint64_t>;
using bytes = std::vector<uint8_t>;