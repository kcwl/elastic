#pragma once
#include <array>
#include <optional>
#include <vector>

namespace elastic
{
	template <typename _Ty>
	class fixed
	{
	public:
		fixed()
			: value_()
			, has_value_(false)
		{}

	public:
		using value_type = _Ty;

		template <typename _Integer>
		fixed& operator=(const _Integer& other)
		{
			value_ = other;

			return *this;
		}

		template <typename _U>
		bool operator==(const fixed<_U>& other) const
		{
			return value_ == other.value_;
		}

		void emplace(value_type&& value)
		{
			value_ = value;

			has_value_ = true;
		}

		bool has_value()
		{
			return has_value_;
		}

		constexpr const value_type& operator*()
		{
			return value_;
		}

	private:
		value_type value_;

		bool has_value_;
	};
} // namespace elastic