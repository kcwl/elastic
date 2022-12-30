#pragma once
#include <elastic/detail/concepts.hpp>
#include <optional>
#include <vector>

namespace elastic
{
	namespace attr
	{
		template <typename _Ty>
		struct fixed
		{
			using value_type = _Ty;

			template <typename _Integer>
			fixed& operator=(const _Integer& other)
			{
				value_ = other;

				return *this;
			}

			value_type value_;
		};

		template <bool flag, typename _Ty>
		struct basic_optional : std::optional<_Ty>
		{
			constexpr static bool require_value = false;
		};

		template <typename _Ty>
		struct basic_optional<true, _Ty> : std::optional<_Ty>
		{
			constexpr static bool require_value = true;
		};
	} // namespace attr

	using fixed32 = attr::fixed<uint32_t>;
	using fixed64 = attr::fixed<uint64_t>;

	template <typename _Ty>
	struct is_fixed : std::false_type
	{};

	template <typename _Ty>
	struct is_fixed<attr::fixed<_Ty>> : std::true_type
	{};

	template <typename _Ty>
	using optional = attr::basic_optional<false, _Ty>;

	template <typename _Ty>
	using repeated = std::vector<_Ty>;

	template <typename _Ty>
	using require = attr::basic_optional<true, _Ty>;

	template <typename _Ty1, typename _Ty2, typename _Result>
	struct second_type : std::bool_constant<std::is_same_v<_Ty1, _Ty2>>
	{
		using type = _Result;
	};

	template <detail::signed_integer _Ty>
	struct unsign
	{
		using value_type =
			typename std::disjunction<second_type<_Ty, int8_t, uint8_t>, second_type<_Ty, int16_t, uint16_t>,
									  second_type<_Ty, int32_t, uint32_t>, second_type<_Ty, int64_t, uint64_t>>::type;

		template <typename _Integer>
		unsign& operator=(const _Integer& other)
		{
			value_ = other;

			return *this;
		}

		value_type value_;
	};

	template <typename _Ty>
	struct is_unsign : std::false_type
	{};

	template <typename _Ty>
	struct is_unsign<unsign<_Ty>> : std::true_type
	{};

	template <typename _Ty>
	concept optional_t = requires(_Ty value) {
							 value.has_value();
							 *value;
						 };

	template <typename _Ty>
	concept fixed_t = is_fixed<_Ty>::value;

	template <typename _Ty>
	concept unsign_t = is_unsign<_Ty>::value;

	template <typename _Ty>
	concept attribute = optional_t<std::remove_cvref_t<_Ty>> || fixed_t<std::remove_cvref_t<_Ty>> || unsign_t<std::remove_cvref_t<_Ty>>;

} // namespace elastic