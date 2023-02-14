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
		template <std::size_t I, typename _Ty>
		static auto make_element(_Archive& ar)
		{
			auto element = elastic::get<I>(_Ty{});

			ar >> element;

			//if (ar.interpret_state())
				//throw std::runtime_error("make element error!\n");

			return element;
		}

		template <typename _Ty, std::size_t... I>
		static auto pop_element(_Archive& ar, std::index_sequence<I...>)
		{
			return _Ty{ make_element<I, _Ty>(ar)... };
		}

		static _Ty deserialize(_Archive& ar)
		{
			constexpr auto N = elastic::tuple_size_v<_Ty>;

			using Indices = std::make_index_sequence<N>;

			_Ty value = pop_element<_Ty>(ar, Indices{});

			return value;
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

			for (uint16_t i = 0; i < bytes; ++i)
			{
				value.push_back(message<typename _Ty::value_type, _Archive>::template deserialize(ar));
			}

			return value;
		}

		static void serialize(_Ty&& value, _Archive& ar)
		{
			auto bytes = value.size();

			varint<_Archive>::template serialize(std::move(bytes), ar);

			for (auto s : std::forward<_Ty>(value))
			{
				message<typename std::remove_cvref_t<_Ty>::value_type, _Archive>::template serialize(std::move(s), ar);
			}
		}
	};
} // namespace elastic