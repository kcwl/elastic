#pragma once
#include <elastic/detail/basic_int128.hpp>

namespace elastic
{
	using int128_t = detail::basic_int128<int64_t, uint64_t>;
	using uint128_t = detail::basic_int128<uint64_t, uint64_t>;
} // namespace elastic