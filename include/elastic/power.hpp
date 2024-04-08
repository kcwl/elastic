#pragma once
#include "type_traits.hpp"

namespace elastic
{
	template <int Back, int Exponent>
	struct power
	{
		constexpr static std::size_t value = power<Back, Exponent - 1>::value * Back;
	};

	template <int Back>
	struct power<Back, 0>
	{
		constexpr static auto value = 1;
	};
} // namespace elastic