#pragma once
#include "access.hpp"
#include "power.hpp"
#include "reflect.hpp"
#include "type_traits.hpp"
#include <bitset>
#include <vector>

namespace elastic
{
	namespace detail
	{
		template <integer_t _Ty>
		constexpr uint8_t get_symbol(_Ty&& t)
		{
			constexpr auto bit = sizeof(_Ty) * 8 - 1;

			constexpr auto pow = power<2, bit>::value;

			return static_cast<uint8_t>((t & pow) >> bit);
		}

		template <integer_t _Ty>
		constexpr auto get_data(_Ty&& t)
		{
			using type = std::remove_cvref_t<_Ty>;

			constexpr auto pow = power<2, sizeof(_Ty) * 8 - 1>::value - 1;

			return static_cast<type>(t & pow);
		}

		inline uint8_t filter_symbol(uint8_t value)
		{
			constexpr auto bit = sizeof(uint8_t) * 8 - 1;

			constexpr auto pow = power<2, bit>::value;

			return (value & pow) >> bit;
		}

		inline uint8_t filter_negative(uint8_t value)
		{
			constexpr auto bit = sizeof(uint8_t) * 8 - 2;

			constexpr auto pow = power<2, bit>::value;

			return (value & pow) >> bit;
		}

		inline uint8_t filter_length(uint8_t value)
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

			auto symbol = filter_symbol(c);

			auto length = filter_length(c);

			auto negative = filter_negative(c);

			int32_t temp_bit = 0;

			while (length--)
			{
				ar.load(c);
				value += (static_cast<result_t>(c) << temp_bit);
				temp_bit += 8;
			}

			return negative == 0 ? (static_cast<result_t>(symbol) << (sizeof(result_t) * 8 - 1)) | value : ~value + 1;
		}

		template <enum_t _Ty, typename _Archive>
		auto deserialize(_Archive& ar)  -> std::remove_cvref_t<_Ty>
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
			_Ty t{};

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
			_Ty t{};

			using value_type = typename _Archive::value_type;

			std::size_t bytes = deserialize<std::size_t>(ar);

			std::size_t count = bytes;

			while (count--)
			{
				using type = typename _Ty::value_type;

				t.resize(bytes);

				ar.load((value_type*)(t.data() + bytes - count - 1), sizeof(type));
			}

			return t;
		}

		template <sequence_t _Ty, typename _Archive>
		auto deserialize(_Archive& ar) -> std::remove_cvref_t<_Ty>
		{
			_Ty t{};

			using value_type = typename _Archive::value_type;

			std::size_t bytes = deserialize<std::size_t>(ar);

			std::size_t count = bytes;

			while (count--)
			{
				using type = typename _Ty::value_type;

				type value{};

				ar >> value;

				t.push_back(value);
			}

			return t;
		}

		template <integer_t _Ty, typename _Archive>
		void serialize(_Archive& ar, _Ty&& value)
		{
			using value_type = typename _Archive::value_type;

			using result_t = std::remove_cvref_t<_Ty>;

			auto symbol = get_symbol(std::forward<_Ty>(value));

			ar.save(static_cast<uint8_t>(symbol));

			result_t temp;

			int has_negative = 0;

			if (value >= 0) [[likely]]
			{
				temp = get_data(std::forward<_Ty>(value));
			}
			else
			{
				temp = ~value + 1;

				has_negative = 1;
			}

			int bit = 0;

			while (temp)
			{
				value_type bit_data{};

				temp > 0xff ? bit_data = static_cast<value_type>(0xff) : bit_data = static_cast<value_type>(temp);

				ar.save(bit_data);

				temp >>= 8;

				++bit;
			}

			ar.commit(-bit - 1);

			symbol = static_cast<uint8_t>((symbol << 7) | (has_negative << 6) | bit);

			ar.save(symbol);

			ar.commit(bit);
		}

		template <enum_t _Ty, typename _Archive>
		void serialize(_Archive& ar, _Ty&& value)
		{
			return serialize(ar, static_cast<int>(value));
		}

		template <boolean_t _Ty, typename _Archive>
		void serialize(_Archive& ar, _Ty&& value)
		{
			return serialize(ar, static_cast<int>(value));
		}

		template <float_point_t _Ty, typename _Archive>
		void serialize(_Archive& ar, _Ty&& value)
		{
			ar.save(value);
		}

		template <struct_t _Ty, typename _Archive>
		void serialize(_Archive& ar, _Ty&& value)
		{
			reflect::for_each(std::forward<_Ty>(value), [&](auto&& v) { serialize(ar, v); });
		}

		template <string_t _Ty, typename _Archive>
		void serialize(_Archive& ar, _Ty&& value)
		{
			auto bytes = value.size();

			serialize(ar, bytes);

			for (auto& mem : value)
			{
				ar.save(mem);
			}
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
				ar << mem;
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