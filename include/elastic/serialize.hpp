#pragma once
#include "access.hpp"
#include "reflect.hpp"
#include "type_traits.hpp"
#include "zig_zag.hpp"
#include <vector>

namespace elastic
{
	namespace detail
	{
		template <integer_t _Ty, typename _Archive>
		_Ty deserialize(_Archive& ar)
		{
			using zig_t = zig_zag_t<_Ty>;
			zig_t t{};

			using value_type = typename _Archive::value_type;

			value_type c{};

			ar.load(c);

			t = c;

			if (t >= 0x80)
			{
				t -= 0x80;

				int32_t temp_bit = zig_zag_bit;

				while (ar.load(c), (c & 0x80) != 0)
				{
					t += static_cast<zig_t>(c) << temp_bit;
					t -= static_cast<zig_t>(0x80u) << temp_bit;

					temp_bit += zig_zag_bit;
				}

				t += static_cast<zig_t>(c) << temp_bit;
			}

			return zigzag_decode<_Ty>(t);
		}

		template <float_point_t _Ty, typename _Archive>
		_Ty deserialize(_Archive& ar)
		{
			_Ty t{};

			ar.load(t);

			return t;
		}

		template <pod_t _Ty, typename _Archive>
		_Ty deserialize(_Archive& ar)
		{
			constexpr auto N = reflect::tuple_size_v<_Ty>;

			using Indices = std::make_index_sequence<N>;

			auto func = []<std::size_t... I>(_Archive& ar, std::index_sequence<I...>) mutable
			{ return _Ty{ deserialize<reflect::elemet_t<_Ty, I>>(ar)... }; };

			return std::move(func(ar, Indices{}));
		}

		template <sequence_t _Ty, typename _Archive>
		_Ty deserialize(_Archive& ar)
		{
			_Ty t{};

			using value_type = typename _Archive::value_type;

			std::size_t bytes = deserialize<std::size_t>(ar);

			std::size_t count = bytes;

			while (count--)
			{
				using type = typename _Ty::value_type;

				if constexpr (!class_t<type>)
				{
					t.resize(bytes);

					ar.load((value_type*)(t.data() + bytes - count - 1), sizeof(type));
				}
				else
				{
					type value{};
					ar >> value;

					t.push_back(value);
				}
			}

			return t;
		}

		template <integer_t _Ty, typename _Archive>
		void serialize(_Archive& ar, _Ty&& value)
		{
			using type = typename _Archive::value_type;

			auto result = zigzag_encode<_Ty>(std::forward<_Ty>(value));

			while (result >= 0x80)
			{
				ar.save(static_cast<type>(result | 0x80));
				result >>= zig_zag_bit;
			}

			ar.save(static_cast<type>(result));
		}

		template <float_point_t _Ty, typename _Archive>
		void serialize(_Archive& ar, _Ty&& value)
		{
			ar.save(value);
		}

		template <pod_t _Ty, typename _Archive>
		void serialize(_Archive& ar, _Ty&& value)
		{
			reflect::for_each(std::forward<_Ty>(value), [&](auto&& v) { serialize(ar, v); });
		}

		template <sequence_t _Ty, typename _Archive>
		void serialize(_Archive& ar, _Ty&& value)
		{
			using type = std::remove_cvref_t<_Ty>;

			using value_type = typename type::value_type;

			auto bytes = value.size();

			serialize(ar, bytes);

			for (auto& mem : value)
			{
				if constexpr (!class_t<value_type>)
				{
					ar.save(mem);
				}
				else
				{
					ar << mem;
				}
			}
		}
	} // namespace detail

	namespace binary
	{
		template <typename _Archive, typename _Ty>
		inline void deserialize(_Archive& ar, _Ty& t)
		{
			if constexpr (non_inherit_t<_Ty>)
			{
				t = detail::template deserialize<_Ty>(ar);
			}
			else if constexpr (std::is_pointer_v<_Ty>)
			{
			}
			else
			{
				access::template serialize(ar, t);
			}
		}

		template <typename _Archive, typename _Ty>
		inline void serialize(_Archive& ar, _Ty&& t)
		{
			using type = std::remove_reference_t<_Ty>;

			if constexpr (non_inherit_t<type>)
			{
				detail::template serialize(ar, std::forward<_Ty>(t));
			}
			else if constexpr (std::is_pointer_v<_Ty>)
			{
			}
			else
			{
				access::template serialize(ar, std::forward<_Ty>(t));
			}
		}
	} // namespace binary
} // namespace elastic