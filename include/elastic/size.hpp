#pragma once
#include "zig_zag.hpp"

namespace elastic
{
	template <integer_t _Ty>
	std::size_t element_size(const _Ty& t)
	{
		auto temp = zigzag_encode(t);
		std::size_t size = 1;

		while ((temp >>= zig_zag_bit) != 0)
			size++;

		return size;
	}

	template <float_point_t _Ty>
	std::size_t element_size(const _Ty&)
	{
		return sizeof(_Ty);
	}

	template<sequence_t _Ty>
	std::size_t element_size(const _Ty& t);

	template<aggregate_class_t _Ty>
	std::size_t element_size(const _Ty& t)
	{
		constexpr auto N = reflect::tuple_size_v<std::remove_cvref_t<_Ty>>;

		auto func = [&]<std::size_t... I>(std::index_sequence<I...>)
		{
			return (element_size(reflect::get<I>(t)) + ...);
		};

		return func(std::make_index_sequence<N>{});
	}

	template <sequence_t _Ty>
	std::size_t element_size(const _Ty& t)
	{
		using type = std::remove_cvref_t<_Ty>;

		std::size_t size = 0;

		using value_type = typename type::value_type;

		if constexpr (sizeof(value_type) == 1)
		{
			size = t.size();
		}
		else
		{
			for (const auto& value : t)
			{
				size += element_size(value);
			}
		}

		return size;
	}
} // namespace elastic