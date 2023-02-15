#pragma once
#include <cstdint>
#include <optional>
#include <vector>

namespace elastic
{
	using int8 = int8_t;

	using int16 = int16_t;

	using int32 = int32_t;

	using int64 = int64_t;

	using uint8 = uint8_t;

	using uint16 = uint16_t;

	using uint32 = uint32_t;

	using uint64 = uint64_t;

	template <typename _Ty>
	using optional = std::optional<_Ty>;

	namespace ft
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
	} // namespace ft

	using fixed32 = ft::fixed<uint32_t>;
	using fixed64 = ft::fixed<uint64_t>;

	template <typename _Ty>
	using repeated = std::vector<_Ty>;

	using bytes = repeated<std::byte>;

	using string = std::string;

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
	struct is_fixed<ft::fixed<_Ty>> : std::true_type
	{};

	template <typename _Ty>
	concept fixed_t = is_fixed<_Ty>::value;

	template <typename _Ty>
	concept unsign_t = is_unsign<_Ty>::value;

	template <typename _Ty>
	concept attribute_t =
		optional_t<std::remove_cvref_t<_Ty>> || fixed_t<std::remove_cvref_t<_Ty>> || unsign_t<std::remove_cvref_t<_Ty>>;

} // namespace elastic