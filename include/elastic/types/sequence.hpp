#pragma once
#include "../concepts.hpp"
#include "varint.hpp"

namespace elastic
{
	struct sequence
	{
		template <typename _Archive, detail::sequence_t _Ty>
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

		template <typename _Archive, detail::sequence_t _Ty>
		static void serialize(_Archive& ar, _Ty&& value)
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