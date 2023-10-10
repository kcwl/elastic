#pragma once
#include "reflect/filed_count.hpp"

namespace elastic
{
	template <typename _Ty>
	using tuple_size = detail::size_t_<detail::fields_count<_Ty>()>;

	template <typename _Ty>
	constexpr std::size_t tuple_size_v = tuple_size<std::remove_cvref_t<_Ty>>::value;
} // namespace elastic