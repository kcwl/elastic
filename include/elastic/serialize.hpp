#pragma once
#include "access.hpp"
#include "reflect.hpp"
#include "type_traits.hpp"

#include <vector>

namespace
{
	static constexpr int32_t bit = 7;

	template <typename _Ty, typename _Func, std::size_t... I>
	constexpr void for_each(_Ty&& val, _Func&& func, std::index_sequence<I...>)
	{
		return (std::forward<_Func>(func)(elastic::get<I>(std::forward<_Ty>(val))), ...);
	}

	template <typename _Ty, typename _Func, std::size_t N = elastic::tuple_size_v<_Ty>,
			  typename Indices = std::make_index_sequence<N>>
	constexpr void for_each(_Ty&& val, _Func&& func)
	{
		return for_each(std::forward<_Ty>(val), std::forward<_Func>(func), Indices{});
	}

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
		void deserialize(_Archive& ar, _Ty& t)
		{
			using zig_type = zig_zag_t<_Ty>;

			uint8_t c{};

			zig_type result{};

			ar.template load<uint8_t>(c);

			result = c;

			if (result >= 0x80)
			{
				result -= 0x80;

				int8_t temp_bit = bit;

				while (ar.load(c), (c & 0x80) != 0)
				{
					result += static_cast<zig_type>(c) << temp_bit;
					result -= static_cast<zig_type>(0x80u) << temp_bit;

					temp_bit += bit;
				}

				result += static_cast<zig_type>(c) << temp_bit;
			}

			t = static_cast<_Ty>(zigzag_decode<zig_type>(result));
		}

		template <typename _Archive, other_numric_t _Ty>
		void deserialize(_Archive& ar, _Ty& t)
		{
			uint8_t c{};

			uint64_t result{};

			ar.template load<uint8_t>(c);

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

			t = static_cast<_Ty>(result);
		}

		template <typename _Archive, multi_numric_v _Ty>
		void deserialize(_Archive& ar, _Ty& t)
		{
			ar.load(t);
		}

		template <typename _Archive, pod_t _Ty>
		void deserialize(_Archive& ar, _Ty& t)
		{
			if constexpr (fixed_t<std::remove_cvref_t<_Ty>>)
			{
				ar.load(*t);
			}
			else
			{
				constexpr auto N = elastic::tuple_size_v<_Ty>;

				using Indices = std::make_index_sequence<N>;

				for_each(t,
						 [&](auto&& v)
						 {
							 ar >> v;
						 });
			}
		}

		template <typename _Archive, sequence_t _Ty>
		void deserialize(_Archive& ar, _Ty& t)
		{
			std::size_t bytes{};

			deserialize(ar, bytes);

			for (std::size_t i = 0; i < bytes; ++i)
			{
				using value_type = typename _Ty::value_type;

				value_type v{};

				if constexpr (map_t<_Ty>)
				{
					using key_type = std::remove_const_t<decltype(v.first)>;

					key_type key{};

					deserialize(ar, key);
					deserialize(ar, v.second);

					t.insert({key, v.second});
				}
				else
				{
					ar >> v;

					t.push_back(v);
				}
			}
		}

		template <typename _Archive, optional_t _Ty>
		void deserialize(_Archive& ar, _Ty& t)
		{
			using type = typename _Ty::value_type;

			type val{};

			deserialize(ar, val);

			t.emplace(std::move(val));
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
			if constexpr (fixed_t<std::remove_cvref_t<_Ty>>)
			{
				ar.save(*std::forward<_Ty>(value));
			}
			else
			{
				for_each(std::forward<_Ty>(value),
						 [&](auto&& v)
						 {
							 serialize(ar, v);
						 });
			}
		}

		template <typename _Archive, sequence_t _Ty>
		void serialize(_Archive& ar, _Ty&& value)
		{
			using type = std::remove_cvref_t<_Ty>;

			auto bytes = value.size();

			serialize(ar, bytes);

			for (auto& s : std::forward<_Ty>(value))
			{
				if constexpr (map_t<type>)
				{
					serialize(ar, s.first);
					serialize(ar, s.second);
				}
				else
				{
					serialize(ar, s);
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
				impl::template deserialize(ar, t);
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