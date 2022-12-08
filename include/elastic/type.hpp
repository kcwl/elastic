#pragma once
#include <elastic/detail/require.hpp>
#include <optional>
#include <string>

namespace elastic
{
	using int32 = int32_t;

	using uint32 = uint32_t;

	using int64 = int64_t;

	using uint64 = uint64_t;

	using string = std::string;

	template <typename _Ty>
	using optional = std::optional<_Ty>;

	template <typename _Ty>
	using require = detail::require<_Ty>;

	template<typename _Ty>
	using vector = std::vector<_Ty>;
} // namespace elastic