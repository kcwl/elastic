#pragma once
#include "elastic/access.hpp"
#include "elastic/archive_exception.hpp"
#include "elastic/archive_type.hpp"
#include "elastic/binary_iarchive.hpp"
#include "elastic/binary_oarchive.hpp"
#include "elastic/reflect.hpp"
#include "elastic/tuple_size.hpp"

#define ELASTIC_DEFINE(limit, ...)\
friend class elastic::access;
template<typename _Archive>
void serialize(_Archive& ar)
{

}

#define ELASTIC_DEFINE_WITH_BASE(limit, base, ...)

#define ELASTIC_DEFINE_STRUCT(...) ELASTIC_DEFINE(public, __VA_ARGS__)

#define ELASTIC_DEFINE_CLASS(...) ELASTIC_DEFINE(private, __VA_ARGS__)

#define ELASTIC_DEFINE_STRUCT_WITH_BASE(base, ...) ELASTIC_DEFINE_WITH_BASE(public, base, __VA_ARGS__)

#define ELASTIC_DEFINE_CLASS_WITH_BASE(base, ...) ELASTIC_DEFINE_WITH_BASE(private, base, __VA_ARGS__)

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