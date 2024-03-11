#pragma once
#include "access.hpp"
#include "reflect.hpp"

#include <vector>

namespace
{
	static constexpr int32_t zig_zag_bit = 7;

	template <elastic::integer_t _Ty>
	_Ty zigzag_encode(_Ty value)
	{
		constexpr auto size = sizeof(_Ty) * 8 - 1;

		return value << 1 ^ value >> size;
	}

	template <elastic::integer_t _Ty>
	_Ty zigzag_decode(_Ty value)
	{
		return value >> 1 ^ (~(value & 1) + 1);
	}

} // namespace

namespace elastic
{
	namespace detail
	{
		template <integer_t _Ty, typename _Archive>
		_Ty deserialize(_Archive& ar)
		{
			uint64_t t{};

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
					t += static_cast<uint64_t>(c) << temp_bit;
					t -= static_cast<uint64_t>(0x80u) << temp_bit;

					temp_bit += zig_zag_bit;
				}

				t += static_cast<uint64_t>(c) << temp_bit;
			}

			return static_cast<_Ty>(zigzag_decode<uint64_t>(t));
		}

		template <pod_t _Ty, typename _Archive>
		_Ty deserialize(_Archive& ar)
		{
			_Ty t{};

			constexpr auto N = reflect::tuple_size_v<_Ty>;

			using Indices = std::make_index_sequence<N>;

			auto func = []<typename _Archive, std::size_t... I>(_Archive & ar, std::index_sequence<I...>)
			{
				return _Ty{ deserialize<reflect::elemet_t<_Ty, I>>(ar)... };
			};

			t = func(ar, Indices{});

			return t;
		}

		template <sequence_t _Ty, typename _Archive>
		_Ty deserialize(_Archive& ar)
		{
			_Ty t{};

			using value_type = typename _Archive::value_type;

			std::size_t bytes = deserialize<std::size_t>(ar);

			t.resize(bytes);

			std::size_t count = bytes;

			while (bytes--)
			{
				ar.load((value_type*)(t.data() + count - bytes - 1), 1);
			}

			return t;
		}

		template <optional_t _Ty, typename _Archive>
		_Ty deserialize(_Archive& ar)
		{
			_Ty t{};

			using type = typename _Ty::value_type;

			type val{};

			val = deserialize<type>(ar);

			t.emplace(std::move(val));

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

		template <pod_t _Ty, typename _Archive>
		void serialize(_Archive& ar, _Ty&& value)
		{
			reflect::for_each(std::forward<_Ty>(value), [&](auto&& v) { serialize(ar, v); });
		}

		template <sequence_t _Ty, typename _Archive>
		void serialize(_Archive& ar, _Ty&& value)
		{
			auto bytes = value.size();

			serialize(ar, bytes);

			for (auto& mem : value)
			{
				if constexpr (std::same_as<std::string, _Ty>)
				{
					ar.save(mem);
				}
				else
				{
					serialize(ar, mem);
				}
			}
		}

		template <optional_t _Ty, typename _Archive>
		void serialize(_Archive& ar, _Ty&& value)
		{
			serialize(ar, *value);
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