#pragma once
#include "exception.hpp"
#include "concepts.hpp"
#include "reflect.hpp"

#include <vector>

namespace
{
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
} // namespace

namespace elastic
{
	using bytes = std::vector<std::byte>;

	using string = std::string;

	template <typename _Ty>
	using repeated = std::vector<_Ty>;

	template <typename _Ty, std::size_t N>
	using repeate = std::array<_Ty, N>;

	struct varint
	{
		static constexpr int32_t bit = 7;

		template <typename _Archive, single_numric _Ty>
		static void deserialize(_Archive& ar, _Ty& t)
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

		template <typename _Archive, multi_numric _Ty>
		static void deserialize(_Archive& ar, _Ty& t)
		{
			ar.load(t);
		}


		template <typename _Archive, single_numric _Ty>
		static void serialize(_Archive& ar, _Ty&& value)
		{
			using type = relative<_Ty>::type;

			uint64_t result = static_cast<uint64_t>(std::forward<_Ty>(value));

			while (result >= 0x80)
			{
				ar.save(static_cast<uint8_t>(result | 0x80));
				result >>= bit;
			}

			ar.save(static_cast<uint8_t>(result));
		}

		template <typename _Archive, multi_numric _Ty>
		static void serialize(_Archive& ar, _Ty&& value)
		{
			ar.save(std::forward<_Ty>(value));
		}
	};

	struct message
	{
		template <typename _Archive, typename _Ty>
		static void deserialize(_Archive& ar, _Ty& t)
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

		template <typename _Archive, typename _Ty>
		static void serialize(_Archive& ar, _Ty&& value)
		{
			for_each(std::forward<_Ty>(value), [&](auto&& v) { ar << v; });
		}
	};

	struct sequence
	{
		template <typename _Archive, sequence_t _Ty>
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

		template <typename _Archive, sequence_t _Ty>
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