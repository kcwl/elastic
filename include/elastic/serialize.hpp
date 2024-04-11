#pragma once
#include "access.hpp"
#include "integer.hpp"
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

		inline uint8_t filter_negative(const uint8_t value)
		{
			constexpr auto bit = sizeof(uint8_t) * 8 - 2;

			constexpr auto pow = power<2, bit>::value;

			return (value & pow) >> bit;
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

			uint8_t symbol = 0;

			auto negative = filter_negative(c);

			if (negative == 1)
			{
				symbol = 1;
			}
			else
			{
				symbol = filter_symbol(c);
			}

			auto length = filter_length(c);

			// int32_t temp_bit = 0;

			// while (length--)
			//{
			//	ar.load(c);
			//	value += (static_cast<result_t>(c) << temp_bit);
			//	temp_bit += 8;
			// }

			ar.load((value_type*)&value, length);

			return negative == 0 ? (static_cast<result_t>(symbol) << (sizeof(result_t) * 8 - 1)) | value : ~value + 1;
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

			using result_t = std::remove_cvref_t<_Ty>;

			integer<result_t> tag_integer{};

			tag_integer.tag_ = get_symbol(value);

			if constexpr (std::is_unsigned_v<result_t>)
			{
				if (tag_integer.tag_ == 1)
				{
					tag_integer.value_ = get_data(value);

					tag_integer.tag_ <<= 7;
				}
				else
				{
					tag_integer.value_ = value;
				}
			}
			else
			{
				tag_integer.value_ = value;

				if (tag_integer.tag_ == 1) [[unlikely]]
				{
					tag_integer.value_ = ~tag_integer.value_ + 1;

					constexpr uint8_t symbol = 1 << 7 | 1 << 6;

					tag_integer.tag_ = symbol;
				}
			}

			uint8_t bit = 0;

			auto temp = tag_integer.value_;

			while (temp)
			{
				temp >>= 8;

				++bit;
			}

			tag_integer.tag_ |= bit;

			tag_integer.write(ar, bit);

			// ar.save(static_cast<uint8_t>(symbol));

			// result_t temp;

			// int has_negative = 0;

			//

			// int bit = 0;

			// while (temp)
			//{
			//	value_type bit_data{};

			//	temp > 0xff ? bit_data = static_cast<value_type>(0xff) : bit_data = static_cast<value_type>(temp);

			//	ar.save(bit_data);

			//	temp >>= 8;

			//	++bit;
			//}

			// ar.commit(-bit - 1);

			// symbol = static_cast<uint8_t>((symbol << 7) | (has_negative << 6) | bit);

			// ar.save(symbol);

			// ar.commit(bit);
		}

		template <enum_t _Ty, typename _Archive>
		void serialize(_Archive& ar, const _Ty value)
		{
			return serialize(ar, static_cast<int>(value));
		}

		template <boolean_t _Ty, typename _Archive>
		void serialize(_Archive& ar, const _Ty value)
		{
			return serialize(ar, static_cast<int>(value));
		}

		template <float_point_t _Ty, typename _Archive>
		void serialize(_Archive& ar, const _Ty value)
		{
			constexpr auto size = sizeof(_Ty);

			using value_type = typename _Archive::value_type;

			ar.save((const value_type*)&value, size);
		}

		template <struct_t _Ty, typename _Archive>
		void serialize(_Archive& ar, const _Ty& value)
		{
			reflect::visit_each(value, [&ar](auto&&... values) { (serialize(ar, values), ...); });
		}

		template <string_t _Ty, typename _Archive>
		void serialize(_Archive& ar, const _Ty& value)
		{
			using value_type = typename _Archive::value_type;

			auto bytes = value.size();

			serialize(ar, bytes);

			ar.save((const value_type*)value.c_str(), bytes);
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