#pragma once
#include <elastic/serialize/access.hpp>
#include <elastic/serialize/parser.hpp>
#include <elastic/types/types.hpp>

namespace elastic
{
	namespace archive
	{
		template <typename _Archive>
		struct save_non_pointer_type
		{
			struct save_standard
			{
				template <typename _Ty>
				static void invoke(_Archive& ar, _Ty&& t)
				{
					message<_Ty, _Archive>::serialize(std::forward<_Ty>(t), ar);
				}
			};

			struct save_varint
			{
				template <typename _Ty>
				static void invoke(_Archive& ar, _Ty&& t)
				{
					varint<_Archive>::serialize(std::forward<_Ty>(t), ar);
				}
			};

			struct save_string
			{
				template <typename _Ty>
				static void invoke(_Archive& ar, _Ty&& t)
				{
					sequence<_Ty, _Archive>::serialize(t, ar);
				}
			};

			struct save_only
			{
				template <typename _Ty>
				static void invoke(_Archive& ar, _Ty&& t)
				{
					access::serialize(ar, std::forward<_Ty>(t));
				}
			};

			template <typename _Ty>
			static void invoke(_Archive& ar, _Ty&& t)
			{
				using typex = std::conditional_t<
					detail::varint_t<_Ty>, detail::identify_t<save_varint>,
					std::conditional_t<detail::pod<_Ty>, detail::identify_t<save_standard>,
									   std::conditional_t<detail::sequence_t<_Ty>, detail::identify_t<save_string>,
														  detail::identify_t<save_only>>>>;

				typex::invoke(ar, std::forward<_Ty>(t));
			}
		};

		template <typename _Archive>
		struct save_pointer_type
		{
			template <typename _Ty>
			static void invoke(_Archive& ar, const _Ty t)
			{}
		};

		template <typename _Archive>
		struct save_enum_type
		{
			template <typename _Ty>
			static void invoke(_Archive& ar, _Ty&& t)
			{
				ar << static_cast<int>(t);
			}
		};

		template <typename _Archive>
		struct save_optional_type
		{
			template <typename _Ty>
			static void invoke(_Archive& ar, _Ty&& t)
			{
				if constexpr (optional_t<std::remove_cvref_t<_Ty>>)
				{
					ar << *t;
				}
				else
				{
					ar.save(std::move(t).value_);
				}
			}
		};

		template <typename _Archive>
		struct save_array_type
		{
			template <typename _Ty>
			static void invoke(_Archive& ar, _Ty&& t)
			{
				std::size_t c = sizeof(t) / (static_cast<const char*>(static_cast<const void*>(&t[1])) -
											 static_cast<const char*>(static_cast<const void*>(&t[0])));

				ar << c;

				ar << t;
			}
		};

		template <typename _Archive, typename _Ty>
		inline void save(_Archive& ar, _Ty&& t)
		{
			using type = std::remove_reference_t<_Ty>;

			using typex = std::conditional_t<
				std::is_pointer_v<type>, detail::identify_t<save_pointer_type<_Archive>>,
				std::conditional_t<
					std::is_enum_v<type>, detail::identify_t<save_enum_type<_Archive>>,
					std::conditional_t<
						std::is_array_v<type>, detail::identify_t<save_array_type<_Archive>>,
						std::conditional_t<attribute_t<type>, detail::identify_t<save_optional_type<_Archive>>,
										   detail::identify_t<save_non_pointer_type<_Archive>>>>>>;

			typex::invoke(ar, std::forward<_Ty>(t));
		}
	} // namespace serialize
} // namespace elastic