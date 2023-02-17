#pragma once
#include <elastic/detail/concepts.hpp>
#include <elastic/serialize/access.hpp>

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
					varint<_Archive>::template deserialize<_Ty>(ar, t);
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
				t = static_cast<_Ty>(ar.template load<int32_t>());
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
				t.value_ = ar.load<typename _Ty::value_type>();
			}
		};

		template <typename _Archive, typename _Ty>
		inline void load(_Archive& ar, _Ty& t)
		{
			using typex = std::conditional_t<
				std::is_enum_v<_Ty>, detail::identify_t<load_enum_type<_Archive>>,
				std::conditional_t<optional_t<_Ty>, detail::identify_t<laod_optional_type<_Archive>>,
								   std::conditional_t<unsign_t<_Ty> || fixed_t<_Ty>,
													  detail::identify_t<load_unsign_or_fixed_type<_Archive>>,
													  detail::identify_t<load_non_pointer_type<_Archive>>>>>;

			typex::invoke(ar, t);
		}
	} // namespace archive
} // namespace elastic