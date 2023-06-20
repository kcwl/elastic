#pragma once
#include "basic_oprimitive.hpp"
#include "interface_archive.hpp"
#include "oserialize.hpp"

#include <ostream>

namespace elastic
{
	class binary_oarchive : public basic_oprimitive<binary_oarchive, char, std::char_traits<char>>,
							public interface_oarchive<binary_oarchive>
	{
	public:
		template <typename _StreamBuffer>
		requires(std::is_convertible_v<_StreamBuffer, std::streambuf>)
		explicit binary_oarchive(_StreamBuffer& bsb)
			: basic_oprimitive<binary_oarchive, char, std::char_traits<char>>(bsb)
		{}

		binary_oarchive(std::ostream& os)
			: basic_oprimitive<binary_oarchive, char, std::char_traits<char>>(*os.rdbuf())
		{}

	public:
		template <typename _Ty>
		void save_override(_Ty&& t)
		{
			archive::save(*this, std::forward<_Ty>(t));
		}
	};
} // namespace elastic