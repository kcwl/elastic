#pragma once
#include <elastic/detail/type_traits.hpp>

namespace elastic
{
	namespace detail
	{
		template<typename _Ty, std::size_t N>
		concept aggregate_inialize = is_aggregate_initialize<_Ty, N>::value;

		template<typename _Ty>
		concept copable = std::is_copy_constructible_v<std::remove_all_extents_t<_Ty>> &&
								std::is_move_constructible_v<std::remove_all_extents_t<_Ty>> &&
								std::is_move_constructible_v<std::remove_all_extents_t<_Ty>>;


		template<typename _Ty>
		concept reflection = copable<_Ty> && !std::is_polymorphic_v<_Ty> && (std::is_aggregate_v<_Ty> || std::is_scalar_v<_Ty>);

		template<typename _Ty>
		concept tuple = requires()
		{
			std::tuple_size<_Ty>();
		};
	}
}