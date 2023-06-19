#pragma once
#include "binary_iarchive_impl.hpp"

#include <istream>

namespace elastic
{
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
	};
} // namespace elastic