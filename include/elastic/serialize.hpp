#pragma once
#include "access.hpp"
#include "reflect.hpp"
#include "type_traits.hpp"

#include <vector>

namespace
{
	static constexpr int32_t bit = 7;

	template <elastic::signed_numric_t _Ty>
	elastic::zig_zag_t<_Ty> zigzag_encode(_Ty value)
	{
		using type = elastic::zig_zag_t<_Ty>;

		constexpr auto size = sizeof(_Ty) * 8 - 1;

		return static_cast<type>(value) << 1 ^ static_cast<type>(value) >> size;
	}

	template <elastic::unsigned_numric_t _Ty>
	elastic::zig_zag_t<_Ty> zigzag_decode(_Ty value)
	{
		using type = elastic::zig_zag_t<_Ty>;

		return static_cast<type>((value >> 1) ^ (~(value & 1) + 1));
	}

} // namespace

namespace elastic
{
	namespace impl
	{
		template <typename _Archive, signed_numric_t _Ty>
		_Ty deserialize(_Archive& ar)
		{
			_Ty t{};

			using zig_type = zig_zag_t<_Ty>;

			uint8_t c{};

			ar.load(c);

			t = c;

			if (t >= 0x80)
			{
				t -= 0x80;

				int8_t temp_bit = bit;

				while (ar.load(c), (c & 0x80) != 0)
				{
					t += static_cast<zig_type>(c) << temp_bit;
					t -= static_cast<zig_type>(0x80u) << temp_bit;

					temp_bit += bit;
				}

				t += static_cast<zig_type>(c) << temp_bit;
			}

			return t = static_cast<_Ty>(zigzag_decode<zig_type>(t));
		}

		template <typename _Archive, other_numric_t _Ty>
		_Ty deserialize(_Archive& ar)
		{
			_Ty t{};

			uint8_t c{};

			uint64_t result{};

			ar.load(c);

			result = c;

			if (result >= 0x80)
			{
				result -= 0x80;

				int8_t temp_bit = bit;

				while (ar.load(c), (c & 0x80) != 0)
				{
					result += static_cast<uint64_t>(c) << temp_bit;
					result -= static_cast<uint64_t>(0x80) << temp_bit;

					temp_bit += bit;
				}

				result += static_cast<uint64_t>(c) << temp_bit;
			}

			return t = static_cast<_Ty>(result);
		}

		template <typename _Archive, multi_numric_v _Ty>
		_Ty deserialize(_Archive& ar)
		{
			_Ty t{};

			ar.load(t);

			return t;
		}

		template <typename _Archive, typename _Ty, std::size_t... I>
		_Ty deserialize_impl(_Archive& ar, std::index_sequence<I...>)
		{
			return _Ty{ deserialize<_Archive, reflect::elemet_t<_Ty,I>>(ar)... };
		}

		template <typename _Archive, pod_t _Ty>
		_Ty deserialize(_Archive& ar)
		{
			_Ty t{};

			constexpr auto N = reflect::tuple_size_v<_Ty>;

			using Indices = std::make_index_sequence<N>;

			t = deserialize_impl<_Archive, _Ty>(ar, Indices{});

			return t;
		}

		template <typename _Archive, sequence_t _Ty>
		_Ty deserialize(_Archive& ar)
		{
			_Ty t{};

			std::size_t bytes = deserialize<_Archive,std::size_t>(ar);

			t.resize(bytes);

			std::size_t count = bytes;

			while (bytes--)
			{
				ar.load((uint8_t*)(t.data() + count - bytes - 1), 1);
			}

			return t;
		}

		template <typename _Archive, optional_t _Ty>
		_Ty deserialize(_Archive& ar)
		{
			_Ty t{};

			using type = typename _Ty::value_type;

			type val{};

			val = deserialize<_Archive, type>(ar);

			t.emplace(std::move(val));

			return t;
		}

		template <typename _Archive, signed_numric_t _Ty>
		void serialize(_Archive& ar, _Ty&& value)
		{
			auto result = zigzag_encode<_Ty>(std::forward<_Ty>(value));

			while (result >= 0x80)
			{
				ar.save(static_cast<uint8_t>(result | 0x80));
				result >>= bit;
			}

			ar.save(static_cast<uint8_t>(result));
		}

		template <typename _Archive, other_numric_t _Ty>
		void serialize(_Archive& ar, _Ty&& value)
		{
			auto result = static_cast<uint64_t>(std::forward<_Ty>(value));

			while (result >= 0x80)
			{
				ar.save(static_cast<uint8_t>(result | 0x80));
				result >>= bit;
			}

			ar.save(static_cast<uint8_t>(result));
		}

		template <typename _Archive, multi_numric_v _Ty>
		void serialize(_Archive& ar, _Ty&& value)
		{
			ar.save(std::forward<_Ty>(value));
		}

		template <typename _Archive, pod_t _Ty>
		void serialize(_Archive& ar, _Ty&& value)
		{
			reflect::for_each(std::forward<_Ty>(value), [&](auto&& v) { serialize(ar, v); });
		}

		template <typename _Archive, sequence_t _Ty>
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

		template <typename _Archive, optional_t _Ty>
		void serialize(_Archive& ar, _Ty&& value)
		{
			serialize(ar, *value);
		}
	} // namespace impl

	namespace binary
	{
		template <typename _Archive, typename _Ty>
		inline void deserialize(_Archive& ar, _Ty& t)
		{
			if constexpr (non_inherit_t<_Ty>)
			{
				t = impl::template deserialize<_Archive, _Ty>(ar);
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
				impl::template serialize(ar, std::forward<_Ty>(t));
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