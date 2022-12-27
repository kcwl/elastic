#pragma once
#include <elastic/detail/type_traits.hpp>

namespace elastic
{
	namespace serialize
	{
		template<typename _Archive>
		struct load_pointer_type
		{
			template<typename _Ty>
			static void invoke(_Archive& ar, const _Ty t)
			{

			}
		};

		template<typename _Archive>
		struct load_enum_type
		{
			template<typename _Ty>
			static void invoke(_Archive& ar, const _Ty& t)
			{
				ar << static_cast<int>(t);
			}
		};

		template<typename _Archive>
		struct load_array_type
		{
			template<typename _Ty>
			static void invoke(_Archive& ar, const _Ty& t)
			{
				std::size_t c = sizeof(t) / (static_cast<const char*>(static_cast<const void*>(&t[1])) - static_cast<const char*>(static_cast<const void*>(&t[0])));

				ar << c;

				ar << t;
			}
		};

		template<typename _Archive>
		struct load_general_type
		{
			template<typename _Ty>
			static void invoke(_Archive& ar, const _Ty& t)
			{

			}
		};


		template<typename _Archive, typename _Ty>
		inline void load(_Archive& ar, _Ty& t)
		{
			using typex = std::conditional<std::is_pointer_v<_Ty>, detail::identify<load_pointer_type<_Archive>>,
				std::conditional<std::is_enum_v<_Ty>, detail::identify<load_enum_type<_Archive>>,
				std::conditional<std::is_array_v<_Ty>, detail::identify<load_array_type<_Archive>>,
				detail::identify<load_general_type<_Archive>>
				>
				>
			>::type;

			typex::invoke(ar, t);
		}
	}
}