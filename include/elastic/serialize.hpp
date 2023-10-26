#pragma once
#include "access.hpp"
#include "concepts.hpp"
#include "exception.hpp"
#include "reflect.hpp"

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

	template <typename _Ty>
	elastic::zig_zag_t<_Ty> zigzag_encode(_Ty value)
	{
		static_assert(elastic::is_any_of_v<_Ty, int32_t, int64_t>, "zig-zag must be int32 or int64");

		using type = elastic::zig_zag_t<_Ty>;

		constexpr auto size = sizeof(_Ty) * 8;

		return static_cast<type>(value) << 1 ^ static_cast<type>(value) >> (size - 1);
	}

	template <typename _Ty>
	_Ty zigzag_decode(elastic::zig_zag_t<_Ty> value)
	{
		static_assert(elastic::is_any_of_v<_Ty, int32_t, int64_t>, "zig-zag must be int32 or int64");

		return static_cast<_Ty>((value >> 1) ^ (~(value & 1) + 1));
	}

} // namespace

namespace elastic
{
	namespace impl
	{
		template <typename _Archive, signed_numric_v _Ty>
		void deserialize(_Archive& ar, _Ty& t)
		{
			uint8_t c{};

			_Ty result{};

			ar.template load<uint8_t>(c);

			result = c;

			if (result >= 0x80)
			{
				result -= 0x80;

				int8_t temp_bit = bit;

				while (ar.load(c), (c & 0x80) != 0)
				{
					result += c << temp_bit;
					result -= static_cast<uint8_t>(0x80u) << temp_bit;

					temp_bit += bit;
				}

				result += static_cast<uint64_t>(c) << temp_bit;
			}

			t = zigzag_decode<_Ty>(result);
		}

		template <typename _Archive, unsigned_numric_v _Ty>
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
			constexpr auto N = elastic::tuple_size_v<_Ty>;

			using Indices = std::make_index_sequence<N>;

			for_each(t,
					 [&](auto&& v)
					 {
						 ar >> v;

						 if (ar.interrupt())
							 throw(archive_exception::exception_number::output_stream_error, "make element error!");
					 });
		}

		template <typename _Archive, sequence_t _Ty>
		void deserialize(_Archive& ar, _Ty& t)
		{
			uint32_t bytes{};
			deserialize(ar, bytes);

			t.resize(bytes);

			for (auto& v : t)
			{
				ar >> v;
			}
		}

		template <typename _Archive, property_t _Ty>
		void deserialize(_Archive& ar, _Ty& t)
		{
			using type = typename _Ty::value_type;

			type val{};

			ar >> val;

			t.emplace(std::move(val));
		}

		template <typename _Archive, signed_numric_v _Ty>
		void serialize(_Archive& ar, _Ty&& value)
		{
			using type = relative<_Ty>::type;

			auto result = zigzag_encode<_Ty>(std::forward<_Ty>(value));

			while (result >= 0x80)
			{
				ar.save(static_cast<uint8_t>(result | 0x80));
				result >>= bit;
			}

			ar.save(static_cast<uint8_t>(result));
		}

		template <typename _Archive, unsigned_numric_v _Ty>
		void serialize(_Archive& ar, _Ty&& value)
		{
			using type = relative<_Ty>::type;

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
			for_each(std::forward<_Ty>(value), [&](auto&& v) { ar << v; });
		}

		template <typename _Archive, sequence_t _Ty>
		void serialize(_Archive& ar, _Ty&& value)
		{
			auto bytes = value.size();

			ar << bytes;

			for (auto& s : std::forward<_Ty>(value))
			{
				ar << s;
			}
		}

		template <typename _Archive, property_t _Ty>
		void serialize(_Archive& ar, _Ty&& value)
		{
			ar << *value;
		}
	} // namespace impl

	template <typename _Archive, typename _Ty>
	inline void binary_load(_Archive& ar, _Ty& t)
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
	inline void binary_save(_Archive& ar, _Ty&& t)
	{
		using type = std::remove_reference_t<_Ty>;

		if constexpr (non_inherit_t<type>)
		{
			impl::template serialize(ar, t);
		}
		else if constexpr (std::is_pointer_v<_Ty>)
		{
		}
		else
		{
			access::template serialize(ar, t);
		}
	}
} // namespace elastic