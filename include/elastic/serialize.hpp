#pragma once
#include "access.hpp"
#include "power.hpp"
#include "reflect.hpp"
#include "type_traits.hpp"
#include <bitset>
#include <vector>

namespace elastic
{
	namespace binary
	{
		template <typename _Archive, typename _Ty>
		void serialize(_Archive& ar, const _Ty& t);
	}

	namespace detail
	{
		template <integer_t _Ty>
		constexpr uint8_t get_symbol(const _Ty t)
		{
			constexpr auto bit = sizeof(_Ty) * 8 - 1;

			constexpr auto pow = power<2, bit>::value;

			return static_cast<uint8_t>((t & pow) >> bit);
		}

		template <integer_t _Ty>
		constexpr auto get_data(const _Ty t)
		{
			constexpr auto pow = power<2, sizeof(_Ty) * 8 - 1>::value - 1;

			return static_cast<_Ty>(t & pow);
		}

		inline uint8_t filter_symbol(const uint8_t value)
		{
			constexpr auto bit = sizeof(uint8_t) * 8 - 1;

			constexpr auto pow = power<2, bit>::value;

			return static_cast<uint8_t>((value & pow) >> bit);
		}

		inline uint8_t filter_length(const uint8_t value)
		{
			constexpr auto pow = power<2, 0>::value + power<2, 1>::value + power<2, 2>::value + power<2, 3>::value;

			return value & pow;
		}

		template <integer_t _Ty, typename _Archive>
		auto deserialize(_Archive& ar) -> std::remove_cvref_t<_Ty>
		{
			using value_type = typename _Archive::value_type;

			using result_t = std::remove_cvref_t<_Ty>;

			value_type c{};
			ar.load(c);

			result_t value{};

			uint8_t symbol = symbol = filter_symbol(c);

			auto length = filter_length(c);

			// int32_t temp_bit = 0;

			// while (length--)
			//{
			//	ar.load(c);
			//	value += (static_cast<result_t>(c) << temp_bit);
			//	temp_bit += 8;
			// }

			ar.load((value_type*)&value, length);

			return symbol == 0 ? value : ~value + 1;
		}

		template <enum_t _Ty, typename _Archive>
		auto deserialize(_Archive& ar) -> std::remove_cvref_t<_Ty>
		{
			return static_cast<_Ty>(deserialize<int>(ar));
		}

		template <boolean_t _Ty, typename _Archive>
		auto deserialize(_Archive& ar) -> std::remove_cvref_t<_Ty>
		{
			return deserialize<int>(ar);
		}

		template <float_point_t _Ty, typename _Archive>
		auto deserialize(_Archive& ar) -> std::remove_cvref_t<_Ty>
		{
			using result_t = std::remove_cvref_t<_Ty>;
			result_t t{};

			ar.load(t);

			return t;
		}

		template <struct_t _Ty, typename _Archive>
		auto deserialize(_Archive& ar) -> std::remove_cvref_t<_Ty>
		{
			constexpr auto N = reflect::tuple_size_v<_Ty>;

			using Indices = std::make_index_sequence<N>;

			using result_t = std::remove_cvref_t<_Ty>;

			auto func = []<std::size_t... I>(_Archive& ar, std::index_sequence<I...>) mutable
			{ return result_t{ deserialize<reflect::elemet_t<_Ty, I>>(ar)... }; };

			return func(ar, Indices{});
		}

		template <string_t _Ty, typename _Archive>
		auto deserialize(_Archive& ar) -> std::remove_cvref_t<_Ty>
		{
			using result_t = std::remove_cvref_t<_Ty>;

			result_t t{};

			using value_type = typename _Archive::value_type;

			std::size_t bytes = deserialize<std::size_t>(ar);

			t.resize(bytes);

			ar.load((value_type*)t.data(), bytes);

			return t;
		}

		template <sequence_t _Ty, typename _Archive>
		auto deserialize(_Archive& ar) -> std::remove_cvref_t<_Ty>
		{
			using result_t = std::remove_cvref_t<_Ty>;

			result_t t{};

			std::size_t bytes = deserialize<std::size_t>(ar);

			std::size_t count = bytes;

			while (count--)
			{
				using type = typename result_t::value_type;

				type value;

				ar >> value;

				t.push_back(value);
			}

			return t;
		}

		template <integer_t _Ty, typename _Archive>
		void serialize(_Archive& ar, const _Ty value)
		{
			using value_type = typename _Archive::value_type;

			uint8_t symbol;

			_Ty result = value;

			if constexpr (!std::is_unsigned_v<_Ty>)
			{
				symbol = get_symbol(value);

				if (symbol & 1)
				{
					result = ~result + 1;
				}
			}

			int8_t bit = 0;

			auto temp = result;

			while (temp != 0)
			{
				temp >>= 8;
				++bit;
			}

			symbol = symbol << 7 | bit;

			ar.save(std::span{ &symbol, 1 });
			ar.save(std::span{ (value_type*)&result, static_cast<std::size_t>(bit) });
		}

		template <enum_t _Ty, typename _Archive>
		void serialize(_Archive& ar, const _Ty value)
		{
			return serialize<int>(ar, static_cast<int>(value));
		}

		template <boolean_t _Ty, typename _Archive>
		void serialize(_Archive& ar, const _Ty value)
		{
			return serialize<int>(ar, value);
		}

		template <float_point_t _Ty, typename _Archive>
		void serialize(_Archive& ar, const _Ty value)
		{
			constexpr auto size = sizeof(_Ty);

			using value_type = typename _Archive::value_type;

			ar.save(std::span{ (value_type*)&value, size });
		}

		template <struct_t _Ty, typename _Archive>
		void serialize(_Archive& ar, const _Ty& value)
		{
			reflect::visit_each(value, [&ar](auto&&... values) { (serialize(ar, std::move(values)), ...); });
		}

		template <string_t _Ty, typename _Archive>
		void serialize(_Archive& ar, const _Ty& value)
		{
			using value_type = typename _Archive::value_type;

			auto bytes = value.size();

			serialize(ar, bytes);

			ar.save(value);
		}

		template <sequence_t _Ty, typename _Archive>
		void serialize(_Archive& ar, const _Ty& value)
		{
			using value_type = typename _Ty::value_type;

			auto bytes = value.size();

			serialize(ar, bytes);

			for (auto& mem : value)
			{
				binary::serialize(ar, mem);
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
		inline void serialize(_Archive& ar, const _Ty& t)
		{
			if constexpr (non_inherit_t<_Ty>)
			{
				detail::template serialize(ar, t);
			}
			else if constexpr (std::is_pointer_v<_Ty>)
			{
			}
			else
			{
				access::template serialize(ar, t);
			}
		}
	} // namespace binary
} // namespace elastic