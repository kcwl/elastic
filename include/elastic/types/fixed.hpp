#pragma once
#include <cstdint>

namespace elastic
{
	template <typename _Ty>
	struct fixed
	{
		using value_type = _Ty;

		template <typename _Integer>
		fixed& operator=(const _Integer& other)
		{
			value_ = other;

			return *this;
		}

		value_type value_;

		template <typename _U>
		bool operator==(const fixed<_U>& other) const
		{
			return value_ == other.value_;
		}
	};

	using fixed32 = fixed<uint32_t>;
	using fixed64 = fixed<uint64_t>;
} // namespace elastic