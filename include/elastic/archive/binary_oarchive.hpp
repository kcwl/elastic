#pragma once
#include <elastic/archive/binary_oarchive_impl.hpp>
#include <ostream>

namespace elastic
{
	class binary_oarchive
		: public binary_oarchive_impl<binary_oarchive, std::ostream::char_type, std::ostream::traits_type>
	{
	public:
		explicit binary_oarchive(std::ostream& os)
			: binary_oarchive_impl<binary_oarchive, std::ostream::char_type, std::ostream::traits_type>(os)
		{}

		explicit binary_oarchive(std::streambuf& bsb)
			: binary_oarchive_impl<binary_oarchive, std::ostream::char_type, std::ostream::traits_type>(bsb)
		{}
	};

} // namespace elastic