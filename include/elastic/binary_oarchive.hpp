#pragma once
#include <elastic/binary_oarchive_impl.hpp>
#include <elastic/oserialize.hpp>
#include <ostream>

namespace elastic
{
	namespace detail
	{
		// template <typename _Archive, typename _Elem, typename _Traits>
		// class binary_oarchive_impl : public binary_oprimitive<_Archive, _Elem, _Traits>
		//{
		// public:
		//	explicit binary_oarchive_impl(std::basic_streambuf<_Elem, _Traits>& sb)
		//		: binary_oprimitive<_Archive, _Elem, _Traits>(sb)
		//	{}

		//	binary_oarchive_impl(std::ostream& os)
		//		: binary_oprimitive<_Archive, _Elem, _Traits>(*os.rdbuf())
		//	{}

		// public:
		//	template <typename _Ty>
		//	_Archive& operator<<(_Ty&& t)
		//	{
		//		serialize::save(*this->archive(), std::forward<_Ty>(t));

		//		return *this->archive();
		//	}

		//	template <typename _Ty>
		//	_Archive& operator&(const _Ty& t)
		//	{
		//		*this->archive() << t;

		//		return *this->archive();
		//	}
		//};
	} // namespace detail

	class binary_oarchive
		: public binary_oarchive_impl<binary_oarchive, std::ostream::char_type, std::ostream::traits_type>
	{
	public:
		explicit binary_oarchive(std::ostream& os, bool no_codecvt = true)
			: binary_oarchive_impl<binary_oarchive, std::ostream::char_type, std::ostream::traits_type>(os, no_codecvt)
		{}

		explicit binary_oarchive(std::streambuf& bsb, bool no_codecvt = true)
			: binary_oarchive_impl<binary_oarchive, std::ostream::char_type, std::ostream::traits_type>(bsb, no_codecvt)
		{}
	};

} // namespace elastic