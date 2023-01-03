#pragma once
#include <elastic/binary_primitive.hpp>
#include <elastic/iserialize.hpp>
#include <elastic/serialize_streambuf.hpp>
#include <istream>

namespace elastic
{
	template <typename _Archive, typename _Elem, typename _Traits>
	class binary_iarchive_impl : public binary_iprimitive<_Archive, _Elem, _Traits>
	{
	public:
		binary_iarchive_impl(std::basic_streambuf<_Elem, _Traits>& sb)
			: binary_iprimitive<_Archive, _Elem, _Traits>(sb)
		{}

		binary_iarchive_impl(std::istream& is)
			: binary_iprimitive<_Archive, _Elem, _Traits>(*is.rdbuf())
		{}

	public:
		template <typename _Ty>
		_Archive& operator>>(_Ty& t)
		{
			try
			{
				this->start();

				serialize::load(*this->archive(), t);
			}
			catch (...)
			{
				this->roll_back();
			}

			return *this->archive();
		}

		template <typename _Ty>
		_Archive& operator&(_Ty& t)
		{
			*this->archive() >> t;

			return *this->archive();
		}
	};

	class binary_iarchive
		: public binary_iarchive_impl<binary_iarchive, std::istream::char_type, std::istream::traits_type>
	{
	public:
		explicit binary_iarchive(std::istream& is)
			: binary_iarchive_impl<binary_iarchive, std::istream::char_type, std::istream::traits_type>(is)
		{}

		explicit binary_iarchive(std::streambuf& bsb)
			: binary_iarchive_impl<binary_iarchive, std::istream::char_type, std::istream::traits_type>(bsb)
		{}

		explicit binary_iarchive(serialize_streambuf<std::istream::char_type, std::istream::traits_type>& sb)
			: binary_iarchive_impl<binary_iarchive, std::istream::char_type, std::istream::traits_type>(sb)
		{}
	};
} // namespace elastic