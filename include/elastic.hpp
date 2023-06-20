#pragma once
#include "elastic/access.hpp"
#include "elastic/binary_iarchive.hpp"
#include "elastic/binary_oarchive.hpp"
#include "elastic/reflect.hpp"
#include "elastic/tuple_size.hpp"
#include "elastic/types/types.hpp"
#include "elastic/archive_exception.hpp"

namespace elastic
{
	template <typename _Ty, typename _Buffer>
	void to_binary(_Ty&& t, _Buffer& buffer)
	{
		binary_oarchive oa(buffer);

		oa << std::forward<_Ty>(t);
	}

	template <typename _Ty, typename _Buffer>
	void from_binary(_Ty& t, _Buffer& buffer)
	{
		binary_iarchive ia(buffer);

		ia >> t;
	}
} // namespace elastic