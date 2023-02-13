#pragma once
#include <cstdint>
#include <elastic/detail/concepts.hpp>
#include <elastic/level_enum.hpp>
#include <elastic/extended_type_info_typeid.hpp>
#include <elastic/track_emum.hpp>

namespace elastic
{
	struct basic_traits
	{};

	template <typename _Ty, level_type Level, track_type Tracking, uint32_t Version = 0,
			  typename _ETII = extend_type_info_impl<_Ty>,
			  typename Wrapper = std::false_type>
	struct traits : basic_traits
	{
		static_assert(Version == 0 || Level >= level_type::object_class_info);
		static_assert(Tracking == track_type::track_never || Level >= level_type::object_serializable);

		using level = std::integral_constant<level_type,Level>;
		using tracking = std::integral_constant<track_type,Tracking>;
		using version = std::integral_constant<uint32_t, Version>;
		using type_info_implementation = _ETII;
		using is_wrapper = Wrapper;
	};

}