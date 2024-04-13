#pragma once
#include "access.hpp"
#include "bytes.hpp"
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
		void serialize(_Archive& ar, _Ty&& t);
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

		// template <integer_t _Ty, typename _Archive>
		// void deserialize(_Archive& ar, _Ty& t)
		//{
		//
		// }

		// template <float_point_t _Ty, typename _Archive>
		// void deserialize(_Archive& ar, _Ty& t)
		//{
		//	ar.load(t);
		// }

		// template <struct_t _Ty, typename _Archive>
		// void deserialize(_Archive& ar, _Ty& t)
		//{
		//	// constexpr auto N = reflect::tuple_size_v<_Ty>;

		//	// using Indices = std::make_index_sequence<N>;

		//	// using result_t = std::remove_cvref_t<_Ty>;

		//	// auto func = []<std::size_t... I>(_Archive& ar, std::index_sequence<I...>) mutable
		//	//{ return result_t{ deserialize<reflect::elemet_t<_Ty, I>>(ar)... }; };

		//	// return func(ar, Indices{});

		//	//reflect::visit_each(t, [&ar](auto&... values) mutable { (deserialize(ar, values), ...); });
		//	access::template serialize(ar, t);
		//}

		// template <string_t _Ty, typename _Archive>
		// void deserialize(_Archive& ar, _Ty& t)
		//{
		//	using value_type = typename _Archive::value_type;

		//	std::size_t bytes{};

		//	deserialize<std::size_t>(ar, bytes);

		//	t.resize(bytes);

		//	ar.load((value_type*)t.data(), bytes);
		//}

		// template <sequence_t _Ty, typename _Archive>
		// void deserialize(_Archive& ar, _Ty& t)
		//{
		//	std::size_t bytes{};
		//	deserialize<std::size_t>(ar, bytes);

		//	while (bytes--)
		//	{
		//		using type = typename _Ty::value_type;

		//		type value{};

		//		ar >> value;

		//		t.push_back(value);
		//	}
		//}

		template <typename _Ty, typename _Archive>
		void serialize(_Archive& ar, _Ty&& value)
		{
			using type = std::remove_cvref_t<_Ty>;

			if constexpr (integer_t<type>)
			{
				using value_type = typename _Archive::value_type;

				uint8_t symbol = 0;

				type result = value;

				if constexpr (!std::is_unsigned_v<_Ty>)
				{
					symbol = get_symbol(value);

					if (symbol & 1)
					{
						result = ~result + 1;
					}
				}

				std::size_t bit = bytes(result, false);

				symbol = symbol << 7 | static_cast<uint8_t>(bit);

				ar.save(std::span{ &symbol, 1 });
				ar.save(std::span{ (value_type*)&result, bit });
			}
			else if constexpr (boolean_t<type>)
			{
				char result = static_cast<char>(std::forward<_Ty>(value));

				serialize(ar, result);
			}
			else if constexpr (enum_t<type>)
			{
				uint64_t result = static_cast<uint64_t>(std::forward<_Ty>(value));

				serialize(ar, result);
			}
			else if constexpr (float_point_t<type>)
			{
				constexpr auto size = sizeof(_Ty);

				using value_type = typename _Archive::value_type;

				ar.save(std::span{ (value_type*)&value, size });
			}
			else if constexpr (string_t<type>)
			{
				using value_type = typename _Archive::value_type;

				auto bytes = value.size();

				serialize(ar, bytes);

				ar.save(std::span{ (value_type*)value.data(), value.size() });
			}
			else if constexpr (sequence_t<type>)
			{
				using value_type = typename std::remove_cvref_t<_Ty>::value_type;

				const auto bytes = value.size();

				serialize(ar, bytes);

				for (auto& mem : value)
				{
					binary::serialize(ar, mem);
				}
			}
		}

		template <typename _Ty, typename _Archive>
		void deserialize(_Archive& ar, _Ty& t)
		{
			using value_type = typename _Archive::value_type;

			if constexpr (integer_t<_Ty>)
			{
				value_type c{};
				ar.load((value_type*)&c, 1);

				uint8_t symbol = filter_symbol(c);

				auto length = filter_length(c);

				ar.load((value_type*)&t, length);

				symbol == 0 ? t : t = ~t + 1;
			}
			else if constexpr (boolean_t<_Ty>)
			{
				char temp{};

				deserialize(ar, temp);

				t = static_cast<bool>(temp);
			}
			else if constexpr (enum_t<_Ty>)
			{
				uint64_t temp{};

				deserialize(ar, temp);

				t = static_cast<_Ty>(temp);
			}
			else if constexpr (float_point_t<_Ty>)
			{
				constexpr auto size = sizeof(_Ty);

				ar.load((value_type*)&t, size);
			}
			else if constexpr (string_t<_Ty>)
			{
				int bytes{};

				deserialize(ar, bytes);

				t.resize(bytes);

				ar.load((value_type*)t.data(), bytes);
			}
			else if constexpr (sequence_t<_Ty>)
			{
				int bytes{};

				deserialize(ar, bytes);

				while (bytes--)
				{
					using value_type = typename _Ty::value_type;

					value_type value{};

					ar >> value;

					t.push_back(value);
				}
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
				detail::template deserialize<_Ty>(ar, t);
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
			if constexpr (non_inherit_t<_Ty>)
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