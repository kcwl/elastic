#pragma once
#include <vector>
#include <array>
#include <optional>

namespace elastic
{
	template <typename _Ty>
	using repeated = std::vector<_Ty>;

	template<typename _Ty, std::size_t N>
	using repeate = std::array<_Ty,N>;

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

	template<typename _Ty>
	using optional = std::optional<_Ty>;
}