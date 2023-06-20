#pragma once
#include "basic_iprimitive.hpp"
#include "binary_iserialize.hpp"
#include "interface_archive.hpp"

#include <istream>

namespace elastic
{
	class binary_iarchive : public basic_iprimitive<binary_iarchive, char, std::char_traits<char>>,
							public interface_iarchive<binary_iarchive>
	{
	public:
		template <typename _StreamBuffer>
		requires(std::is_convertible_v<_StreamBuffer, std::streambuf>)
		explicit binary_iarchive(_StreamBuffer& bs)
			: basic_iprimitive<binary_iarchive, char, std::char_traits<char>>(bs)
		{}

		binary_iarchive(std::istream& is)
			: basic_iprimitive<binary_iarchive, char, std::char_traits<char>>(*is.rdbuf())
		{}

	public:
		template <typename _Ty>
		void load_override(_Ty& t)
		{
			try
			{
				this->start();

				archive::binary_load(*this, t);
			}
			catch (...)
			{
				this->roll_back();
			}
		}
	};

} // namespace elastic