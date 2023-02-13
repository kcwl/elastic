#pragma once
#include <elastic/iserialize.hpp>
#include <elastic/serialize_streambuf.hpp>
#include <istream>
#include <elastic/binary_iarchive_impl.hpp>

namespace elastic
{
	//template <typename _Archive, typename _Elem, typename _Traits>
	//class binary_iarchive_impl : public binary_iprimitive<_Archive, _Elem, _Traits>
	//{
	//public:
	//	binary_iarchive_impl(std::basic_streambuf<_Elem, _Traits>& sb)
	//		: binary_iprimitive<_Archive, _Elem, _Traits>(sb)
	//	{}

	//	binary_iarchive_impl(std::istream& is)
	//		: binary_iprimitive<_Archive, _Elem, _Traits>(*is.rdbuf())
	//	{}

	//public:
	//	template <typename _Ty>
	//	_Archive& operator>>(_Ty& t)
	//	{
	//		try
	//		{
	//			this->start();

	//			serialize::load(*this->archive(), t);
	//		}
	//		catch (...)
	//		{
	//			this->archive()->roll_back();
	//		}

	//		return *this->archive();
	//	}

	//	template <typename _Ty>
	//	_Archive& operator&(_Ty& t)
	//	{
	//		*this->archive() >> t;

	//		return *this->archive();
	//	}
	//};

	class binary_iarchive
		: public binary_iarchive_impl<binary_iarchive, std::istream::char_type, std::istream::traits_type>
	{
	public:
		explicit binary_iarchive(std::istream& is, bool no_codecvt = true)
			: binary_iarchive_impl<binary_iarchive, std::istream::char_type, std::istream::traits_type>(is, no_codecvt)
		{}

		explicit binary_iarchive(std::streambuf& bsb, bool no_codecvt = true)
			: binary_iarchive_impl<binary_iarchive, std::istream::char_type, std::istream::traits_type>(bsb, no_codecvt)
		{}
	};
} // namespace elastic