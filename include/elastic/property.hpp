#pragma once
#include <vector>
#include <array>
#include <optional>

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

		fixed& operator*()
		{
			return value_;
		}
	};

	template<typename _Ty>
	using optional = std::optional<_Ty>;
}