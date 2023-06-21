#pragma once
#include "../detail/concepts.hpp"

namespace elastic
{
	struct float_t
	{
		template <typename _Archive, detail::multi_numric _Ty>
		static void deserialize(_Archive& ar, _Ty& t)
		{
			return ar.load<_Ty>(t);
		}

		template <typename _Archive, detail::multi_numric _Ty>
		static void serialize(_Archive& ar, _Ty&& value)
		{
			ar.save(std::forward<_Ty>(value));
		}
	};
} // namespace elastic