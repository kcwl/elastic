#pragma once
#include <elastic/access.hpp>
#include <elastic/detail/type_traits.hpp>

namespace elastic
{
	namespace serialize
	{
		template <typename _Archive>
		struct load_non_pointer_type
		{

			struct load_standard
			{
				template <typename _Ty>
				static void invoke(_Archive& ar, _Ty& t)
				{
					t = message<_Ty, _Archive>::parse_binary(ar);
				}
			};

			struct load_varint
			{
				template <typename _Ty>
				static void invoke(_Archive& ar, _Ty& t)
				{
					t = varint<_Archive>::template parse_binary<_Ty>(ar);
				}
			};

			struct load_string
			{
				template <typename _Ty>
				static void invoke(_Archive& ar, _Ty& t)
				{
					t = sequence<_Ty, _Archive>::template parse_binary(ar);
				}
			};

			template <typename _Ty>
			static void invoke(_Archive& ar, _Ty& t)
			{
				using typex = std::conditional_t<detail::pod<_Ty>, detail::identify_t<load_standard>,
												 detail::identify_t<load_string>>;

				typex::invoke(ar, t);
			}
		};

		template <typename _Archive>
		struct load_pointer_type
		{
			template <typename _Ty>
			static void invoke(_Archive& ar, _Ty t)
			{}
		};

		template <typename _Archive>
		struct load_enum_type
		{
			template <typename _Ty>
			static void invoke(_Archive& ar, _Ty& t)
			{
				int value{};

				load_non_pointer_type<_Archive>::load_varint::template invoke<int>(ar, value);

				t = static_cast<_Ty>(value);
			}
		};

		template <typename _Archive>
		struct load_array_type
		{
			template <typename _Ty>
			static void invoke(_Archive& ar, _Ty& t)
			{}
		};

		template <typename _Archive>
		struct laod_optional_type
		{
			template <typename _Ty>
			static void invoke(_Archive& ar, _Ty& t)
			{
				using type = typename _Ty::value_type;

				type val{};

				ar >> val;

				t.emplace(val);
			}
		};

		template <typename _Archive>
		struct load_unsign_or_fixed_type
		{
			template <typename _Ty>
			static void invoke(_Archive& ar, _Ty& t)
			{
				t.value_ = ar.read<typename _Ty::value_type>();
			}
		};

		template <typename _Archive, typename _Ty>
		inline void load(_Archive& ar, _Ty& t)
		{
			using typex = std::conditional_t<
				std::is_pointer_v<_Ty>, detail::identify_t<load_pointer_type<_Archive>>,
				std::conditional_t<
					std::is_enum_v<_Ty>, detail::identify_t<load_enum_type<_Archive>>,
					std::conditional_t<
						std::is_array_v<_Ty>, detail::identify_t<load_array_type<_Archive>>,
						std::conditional_t<optional_t<_Ty>, detail::identify_t<laod_optional_type<_Archive>>,
										   std::conditional_t<unsign_t<_Ty> || fixed_t<_Ty>,
															  detail::identify_t<load_unsign_or_fixed_type<_Archive>>,
															  detail::identify_t<load_non_pointer_type<_Archive>>>>>>>;

			typex::invoke(ar, t);
		}
	} // namespace serialize
} // namespace elastic