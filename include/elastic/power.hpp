#pragma once
#include "type_traits.hpp"

namespace elastic
{
	template<std::size_t Back, std::size_t Exponent>
	struct power
	{
		constexpr static std::size_t value = power<Back, Exponent - 1>::value * Back;
	};

	template<int64_t Back>
	struct power<Back, 0>
	{
		constexpr static auto value = 1;
	};
}