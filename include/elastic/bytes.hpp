#pragma once
#include "reflect.hpp"
#include "type_traits.hpp"

namespace elastic
{
	template <typename _Ty>
	std::size_t bytes(const _Ty& value)
	{
		std::size_t byte = 0;

		if constexpr (integer_t<_Ty>)
		{
			_Ty temp{};

			value > 0 ? temp = value : temp = ~value + 1;

			byte++;

			while (temp > 0)
			{
				temp >>= 8;

				byte++;
			}
		}
		else if constexpr (enum_t<_Ty> || boolean_t<_Ty>)
		{
			byte = bytes<int>(static_cast<int>(value));
		}
		else if constexpr (float_point_t<_Ty>)
		{
			byte = sizeof(_Ty);
		}
		else if constexpr (struct_t<_Ty>)
		{
			reflect::visit_each(value, [&byte](auto... values) { ((byte += bytes(values)), ...); });
		}
		else if constexpr (string_t<_Ty>)
		{
			auto sz = value.size();

			byte = bytes(sz) + sz;
		}

		return byte;
	}
} // namespace elastic