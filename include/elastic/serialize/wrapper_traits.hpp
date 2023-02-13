#pragma once
#include <elastic/traits.hpp>

namespace elastic
{
	template<typename _Ty, level_type Level=level_type::object_serializable,track_type Tracking = track_type::track_never,uint32_t Version = 0, typename _ETII = extend_type_info_impl<_Ty>>
	struct wrapper_traits : public traits<_Ty, Level, Tracking, Version, _ETII, std::true_type>
	{

	};

	template<typename _Ty>
	struct is_wrapper_impl : std::conditional<std::is_base_of_v<basic_traits, _Ty>, std::true_type, std::false_type>
	{

	};

	template<typename _Ty>
	struct is_wrapper
	{
		using type = is_wrapper_impl<const _Ty>::type;
	};
}