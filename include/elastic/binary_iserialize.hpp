#pragma once
#include "access.hpp"
#include "archive_type.hpp"
#include "detail/concepts.hpp"

namespace elastic
{
	namespace archive
	{
		template <typename _Archive>
		struct load_non_pointer_type
		{
			struct load_only
			{
				template <typename _Ty>
				static void invoke(_Archive& ar, _Ty& t)
				{
					access::serialize(ar, t);
				}
			};

			struct load_standard
			{
				template <typename _Ty>
				static void invoke(_Archive& ar, _Ty& t)
				{
					message<_Ty, _Archive>::template deserialize(ar, t);
				}
			};
			struct load_varint
			{
				template <typename _Ty>
				static void invoke(_Archive& ar, _Ty& t)
				{
					varint::template deserialize(ar, t);
				}
			};

			struct load_sequence
			{
				template <typename _Ty>
				static void invoke(_Archive& ar, _Ty& t)
				{
					sequence<_Ty, _Archive>::template deserialize(ar, t);
				}
			};

			template <typename _Ty>
			static void invoke(_Archive& ar, _Ty& t)
			{
				using typex = std::conditional_t<
					detail::varint_t<_Ty>, detail::identify_t<load_varint>,
					std::conditional_t<detail::pod<_Ty>, detail::identify_t<load_standard>,
									   std::conditional_t<detail::sequence_t<_Ty>, detail::identify_t<load_sequence>,
														  detail::identify_t<load_only>>>>;

				typex::invoke(ar, t);
			}
		};

		template <typename _Archive>
		struct load_enum_type
		{
			template <typename _Ty>
			static void invoke(_Archive& ar, _Ty& t)
			{
				ar.template load<_Ty>(t);
			}
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
				ar.template load<typename _Ty::value_type>(t.value_);
			}
		};

		template <typename _Archive, typename _Ty>
		inline void binary_load(_Archive& ar, _Ty& t)
		{
			using typex = std::conditional_t<
				std::is_enum_v<_Ty>, detail::identify_t<load_enum_type<_Archive>>,
				std::conditional_t<
					detail::optional_t<_Ty>, detail::identify_t<laod_optional_type<_Archive>>,
					std::conditional_t<detail::fixed_t<_Ty>, detail::identify_t<load_unsign_or_fixed_type<_Archive>>,
									   detail::identify_t<load_non_pointer_type<_Archive>>>>>;

			typex::invoke(ar, t);
		}
	} // namespace archive
} // namespace elastic