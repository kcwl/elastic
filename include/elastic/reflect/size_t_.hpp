#pragma once
#include "concepts.hpp"

namespace elastic
{
	namespace detail
	{
		template <std::size_t N>
		using size_t_ = std::integral_constant<std::size_t, N>;
	}
} // namespace elastic