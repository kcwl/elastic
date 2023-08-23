#pragma once
#include "elastic/access.hpp"
#include "elastic/archive_exception.hpp"
#include "elastic/archive_type.hpp"
#include "elastic/binary_iarchive.hpp"
#include "elastic/binary_oarchive.hpp"
#include "elastic/reflect.hpp"
#include "elastic/tuple_size.hpp"

namespace elastic
{
	template <typename _Ty, typename _Buffer>
	int32_t to_binary(_Ty&& t, _Buffer& buffer)
	{
		auto old_size = buffer.size();

		binary_oarchive oa(buffer);

		oa << std::forward<_Ty>(t);

		return static_cast<int32_t>(old_size - buffer.size());
	}

	template <typename _Ty, typename _Buffer>
	int32_t from_binary(_Ty& t, _Buffer& buffer)
	{
		auto old_size = buffer.size();

		binary_iarchive ia(buffer);

		ia >> t;

		return static_cast<int32_t>(old_size - buffer.size());
	}
} // namespace elastic