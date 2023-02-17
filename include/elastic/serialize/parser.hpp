#pragma once
#include <elastic/reflect.hpp>

namespace
{
	template <typename _Ty, typename Func, std::size_t... I>
	constexpr void for_each(_Ty&& val, Func func, std::index_sequence<I...>)
	{
		return (func(elastic::get<I>(std::forward<_Ty>(val))), ...);
	}

	template <typename _Ty, typename Func, std::size_t N = elastic::tuple_size_v<_Ty>,
			  typename Indices = std::make_index_sequence<N>>
	constexpr void for_each(_Ty&& val, Func func)
	{
		return for_each(std::forward<_Ty>(val), func, Indices{});
	}
} // namespace

namespace elastic
{
	template <typename _Archive>
	struct varint
	{
		template <detail::single_numric _Ty>
		static _Ty deserialize(_Archive& ar)
		{
			uint64_t value = ar.load<uint8_t>();

			if (value > 0x80)
			{
				value -= 0x80;

				uint8_t bit = 7;

				uint8_t c{};
				while (((c = ar.load<uint8_t>()) & 0x80) != 0)
				{
					value += static_cast<uint64_t>(c) << bit;
					value -= static_cast<uint64_t>(0x80) << bit;

					bit += 7;
				}

				value += static_cast<uint64_t>(c) << bit;
			}

			return std::move(static_cast<_Ty>(value));
		}

		template <detail::multi_numric _Ty>
		static _Ty deserialize(_Archive& ar)
		{
			return ar.load<_Ty>();
		}

		template <detail::single_numric _Ty>
		static void serialize(_Ty&& value, _Archive& ar)
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

		template <detail::multi_numric _Ty>
		static void serialize(_Ty&& value, _Archive& ar)
		{
			ar.save(std::forward<_Ty>(value));
		}
	};

	template <typename _Ty, typename _Archive>
	struct message
	{
		static void deserialize(_Archive& ar, _Ty& t)
		{
			constexpr auto N = elastic::tuple_size_v<_Ty>;

			using Indices = std::make_index_sequence<N>;

			for_each(std::move(t),
					 [&](auto&& v)
					 {
						 ar >> v;

						 if (ar.interrupt())
							 throw(archive_exception::exception_number::output_stream_error, "make element error!");
					 });
		}

		static void serialize(_Ty&& value, _Archive& ar)
		{
			for_each(std::forward<_Ty>(value), [&](auto&& v) { ar << v; });
		}
	};

	template <detail::sequence_t _Ty, typename _Archive>
	struct sequence
	{
		static _Ty deserialize(_Archive& ar)
		{
			uint16_t bytes = varint<_Archive>::template deserialize<uint16_t>(ar);

			_Ty value{};

			value.resize(bytes);

			for (auto& v : value)
			{
				ar >> v;
			}

			return value;
		}

		static void serialize(_Ty&& value, _Archive& ar)
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