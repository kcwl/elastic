#pragma once
#include "../detail/concepts.hpp"

namespace elastic
{
	struct varint
	{
		template <typename _Archive, detail::single_numric _Ty>
		static void deserialize(_Archive& ar, _Ty& t)
		{
			uint8_t c{};
			ar.template load<uint8_t>(c);
			t = static_cast<_Ty>(c);

			if (t >= 0x80)
			{
				t -= 0x80;

				uint8_t bit = 7;

				while (ar.load(c), (c & 0x80) != 0)
				{
					t += static_cast<_Ty>(c) << bit;
					t -= static_cast<_Ty>(0x80) << bit;

					bit += 7;
				}

				t += static_cast<uint64_t>(c) << bit;
			}
		}

		template <typename _Archive, detail::single_numric _Ty>
		static void serialize(_Archive& ar, _Ty&& value)
		{
			using type = detail::relative<_Ty>::type;

			uint64_t result = static_cast<type>(std::forward<_Ty>(value));

			while (result > 0x80)
			{
				ar.save(static_cast<uint8_t>(result | 0x80));
				result >>= 7;
			}

			ar.save(static_cast<uint8_t>(result));
		}
	};
} // namespace elastic