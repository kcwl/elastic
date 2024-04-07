#pragma once
#include "reflect.hpp"
#include "type_traits.hpp"

namespace elastic
{
	template <typename _Ty>
	struct bytes
	{
		static std::size_t apply(const _Ty&)
		{
			return 0;
		}
	};

	template <integer_t _Ty>
	struct bytes<_Ty>
	{
		static auto apply(_Ty value)
		{
			std::remove_cvref_t<_Ty> temp{};

			value > 0 ? temp = value : temp = ~value + 1;

			int bytes = 1;

			while (temp > 0)
			{
				temp >>= 8;

				bytes++;
			}

			return bytes;
		}
	};

	template <enum_t _Ty>
	struct bytes<_Ty>
	{
		static auto apply(_Ty value)
		{
			return bytes<int>::apply(static_cast<int>(value));
		}
	};

	template <boolean_t _Ty>
	struct bytes<_Ty>
	{
		static auto apply(_Ty value)
		{
			return bytes<int>::apply(value);
		}
	};

	template <float_point_t _Ty>
	struct bytes<_Ty>
	{
		constexpr static auto apply(_Ty)
		{
			return sizeof(_Ty);
		}
	};

	template <struct_t _Ty>
	struct bytes<_Ty>
	{
		static auto apply(const _Ty& value)
		{
			std::size_t byte = 0;

			reflect::for_each(value,
							  [&](auto&& v)
							  {
								  using type = decltype(v);

								  byte += bytes<type>::apply(v);
							  });

			return byte;
		}
	};

	template <string_t _Ty>
	struct bytes<_Ty>
	{
		static auto apply(const _Ty& value)
		{
			auto sz = value.size();

			return bytes<std::size_t>::apply(sz) + sz;
		}
	};

	template <sequence_t _Ty>
	struct bytes<_Ty>
	{
		static auto apply(const _Ty& value)
		{
			auto sz = value.size();

			return bytes<std::size_t>::apply(sz) + sz;
		}
	};

} // namespace elastic