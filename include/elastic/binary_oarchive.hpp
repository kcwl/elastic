#pragma once
#include <elastic/binary_primitive.hpp>
#include <elastic/oserialize.hpp>
#include <ostream>

namespace elastic
{
	namespace detail
	{
		template<typename _Archive, typename _Elem, typename _Traits>
		class binary_oarchive_impl : public binary_oprimitive<_Archive, _Elem, _Traits>
		{
		public:
			binary_oarchive_impl(std::basic_streambuf<_Elem, _Traits>& sb)
				: binary_oprimitive<_Archive, _Elem, _Traits>(sb)
			{

			}

			binary_oarchive_impl(std::ostream& os)
				: binary_oprimitive<_Archive, _Elem, _Traits>(*os.rdbuf())
			{

			}

		public:
			template<typename _Ty>
			_Archive& operator<<(const _Ty& t)
			{
				serialize::save(this->_this(), t);

				return *this->_this();
			}
		};
	}

	class binary_oarchive
		: public detail::binary_oarchive_impl<binary_oarchive, std::ostream::char_type, std::ostream::traits_type>
	{
	public:
		binary_oarchive(std::ostream& os)
			: detail::binary_oarchive_impl<binary_oarchive, std::ostream::char_type, std::ostream::traits_type>(os)
		{

		}

		binary_oarchive(std::streambuf& bsb)
			: detail::binary_oarchive_impl<binary_oarchive, std::ostream::char_type, std::ostream::traits_type>(bsb)
		{

		}
	};

}