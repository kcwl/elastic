#pragma once
#include "../detail/concepts.hpp"
#include "varint.hpp"

namespace elastic
{
	template <detail::sequence_t _Ty, typename _Archive>
	struct sequence
	{
		static void deserialize(_Archive& ar, _Ty& t)
		{
			uint16_t bytes{};
			varint::template deserialize(ar, bytes);

			t.resize(bytes);

			for (auto& v : t)
			{
				ar >> v;
			}
		}

		static void serialize(_Ty&& value, _Archive& ar)
		{
			auto bytes = value.size();

			ar << bytes;

			for (auto& s : std::forward<_Ty>(value))
			{
				ar << s;
			}
		}
	};
} // namespace elastic