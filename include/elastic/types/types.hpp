#pragma once
#include <cstdint>
#include <optional>
#include <vector>

namespace elastic
{
	namespace numric_type
	{
		using int8 = int8_t;

		using int16 = int16_t;

		using int32 = int32_t;

		using int64 = int64_t;
	} // namespace numric_type

	namespace unsigned_type
	{
		using uint8 = uint8_t;

		using uint16 = uint16_t;

		using uint32 = uint32_t;

		using uint64 = uint64_t;
	} // namespace unsigned_type

	namespace optional_type
	{
		namespace impl
		{
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
		} // namespace impl

		template <typename _Ty>
		using optional = impl::basic_optional<false, _Ty>;

		template <typename _Ty>
		using require = impl::basic_optional<true, _Ty>;
	} // namespace optional_type

	namespace fix_type
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
	} // namespace fix_type

	namespace repeated_type
	{
		template <typename _Ty>
		using repeated = std::vector<_Ty>;

		using bytes = repeated<std::byte>;

		using string = std::string;
	}

	template <typename _Ty>
	struct is_unsign : std::false_type
	{};

	template <typename _Ty>
	concept optional_t = requires(_Ty value) {
							 value.has_value();
							 *value;
						 };

	template <typename _Ty>
	struct is_fixed : std::false_type
	{};

	template <typename _Ty>
	struct is_fixed<fix_type::fixed<_Ty>> : std::true_type
	{};

	template <typename _Ty>
	concept fixed_t = is_fixed<_Ty>::value;

	template <typename _Ty>
	concept unsign_t = is_unsign<_Ty>::value;

	template <typename _Ty>
	concept attribute_t =
		optional_t<std::remove_cvref_t<_Ty>> || fixed_t<std::remove_cvref_t<_Ty>> || unsign_t<std::remove_cvref_t<_Ty>>;

} // namespace elastic