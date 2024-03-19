#pragma once
#include "elastic/access.hpp"
#include "elastic/binary_archive.hpp"
#include "elastic/macro_expand.hpp"
#include "elastic/size.hpp"

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

	template<typename _Ty>
	std::size_t size(const _Ty& t)
	{
		return element_size(t);
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