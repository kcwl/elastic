#pragma once
#include <elastic/detail/type_traits.hpp>

namespace elastic
{
	namespace serialize
	{
		template<typename _Archive>
		struct load_non_pointer_type
		{

			struct load_standard
			{
				template<typename _Ty>
				static void invoke(_Archive& ar, _Ty& t)
				{
					t = message<_Ty, _Archive>::parse_binary(ar);
				}
			};

			struct load_only
			{
				template<typename _Ty>
				static void invoke(_Archive& ar, _Ty& t)
				{
					access::serialize(ar, const_cast<_Ty>(t));
				}
			};

			struct load_varint
			{
				template<typename _Ty>
				static void invoke(_Archive& ar, _Ty& t)
				{
					t = varint<_Archive>::parse_binary(ar);
				}
			};

			struct load_string
			{
				template<typename _Ty>
				static void invoke(_Archive& ar, _Ty& t)
				{
					t = strings<_Ty, _Archive>::parse_binary(ar);
				}
			};

			template<typename _Ty>
			static void invoke(_Archive& ar, _Ty& t)
			{
				using typex = std::conditional_t<detail::pod<_Ty>, detail::identify_t<load_standard>, std::conditional_t<detail::string_t<_Ty>, detail::identify_t<load_string>, detail::identify_t<load_only>> > ;

				typex::invoke(ar, t);
			}
		};


		template<typename _Archive>
		struct load_pointer_type
		{
			template<typename _Ty>
			static void invoke(_Archive& ar, _Ty t)
			{

			}
		};

		template<typename _Archive>
		struct load_enum_type
		{
			template<typename _Ty>
			static void invoke(_Archive& ar, _Ty& t)
			{
				ar << static_cast<int>(t);
			}
		};

		template<typename _Archive>
		struct load_array_type
		{
			template<typename _Ty>
			static void invoke(_Archive& ar, _Ty& t)
			{
				std::size_t c = sizeof(t) / (static_cast<const char*>(static_cast<const void*>(&t[1])) - static_cast<const char*>(static_cast<const void*>(&t[0])));

				ar << c;

				ar << t;
			}
		};


		template<typename _Archive, typename _Ty>
		inline void load(_Archive& ar, _Ty& t)
		{
			using typex = std::conditional_t<std::is_pointer_v<_Ty>, detail::identify_t<load_pointer_type<_Archive>>,
				std::conditional_t<std::is_enum_v<_Ty>, detail::identify_t<load_enum_type<_Archive>>,
				std::conditional_t<std::is_array_v<_Ty>, detail::identify_t<load_array_type<_Archive>>,
				detail::identify_t<load_non_pointer_type<_Archive>>
				>
				>
			>;

			typex::invoke(ar, t);
		}
	}
}