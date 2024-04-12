#pragma once
#include "elastic/access.hpp"
#include "elastic/binary_archive.hpp"
#include "elastic/macro_expand.hpp"
#include "elastic/bytes.hpp"

namespace elastic
{
	template <typename _Ty>
	bool to_binary(const _Ty& t, flex_buffer_t& buffer)
	{
		binary_oarchive oa(buffer);

		oa << t;

		return buffer.success();
	}

	template <typename _Ty, typename _Elem, typename _Traits = std::char_traits<_Elem>>
	void from_binary(_Ty& t, flex_buffer<_Elem, _Traits>& buffer)
	{
		binary_iarchive ia(buffer);

		ia >> t;
	}
} // namespace elastic

#ifndef ELASTIC_ACCESS
#define ELASTIC_ACCESS(...)                                                                                            \
private:                                                                                                               \
	friend class elastic::access;                                                                                      \
	template <typename _Archive>                                                                                       \
	void serialize(_Archive& ar)                                                                                       \
	{                                                                                                                  \
		ELASTIC_BIND_WITH(ar&, __VA_ARGS__);                                                                           \
	}
#endif

#ifndef ELASTIC_ACCESS_IF
#define ELASTIC_ACCESS_IF(base, ...)                                                                                   \
private:                                                                                                               \
	friend class elastic::access;                                                                                      \
	template <typename _Archive>                                                                                       \
	void serialize(_Archive& ar)                                                                                       \
	{                                                                                                                  \
		ar& elastic::base_object<base>(*this);                                                                         \
		ELASTIC_BIND_WITH(ar&, __VA_ARGS__);                                                                           \
	}
#endif