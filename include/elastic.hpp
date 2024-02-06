#pragma once
#include "elastic/access.hpp"
#include "elastic/archive.hpp"


namespace elastic
{
	template <typename _Ty, typename _Elem, typename _Traits = std::char_traits<_Elem>>
	bool to_binary(_Ty&& t, flex_buffer<_Elem, _Traits>& buffer)
	{
		binary_oarchive oa(buffer);

		oa << std::forward<_Ty>(t);

		return !oa.fail();
	}

	template <typename _Ty, typename _Elem, typename _Traits = std::char_traits<_Elem>>
	bool from_binary(_Ty& t, flex_buffer<_Elem, _Traits>& buffer)
	{
		binary_iarchive ia(buffer);

		ia >> t;

		return !ia.fail();
	}
}