#pragma once
#include "interface_archive.hpp"
#include "binary_primitive.hpp"
#include "iserialize.hpp"

#include <istream>

namespace elastic
{
	class binary_iarchive : public binary_iprimitive<binary_iarchive, char, std::char_traits<char>>,
							public interface_iarchive<binary_iarchive>
	{
	public:
		template <typename _StreamBuffer>
		requires(std::is_convertible_v<_StreamBuffer, std::streambuf>)
		explicit binary_iarchive(_StreamBuffer& bs)
			: binary_iprimitive<binary_iarchive, char, std::char_traits<char>>(bs)
		{}

		binary_iarchive(std::istream& is)
			: binary_iprimitive<binary_iarchive, char, std::char_traits<char>>(*is.rdbuf())
		{}

	public:
		template <typename _Ty>
		void load_override(_Ty& t)
		{
			try
			{
				this->start();

				archive::load(*this, t);
			}
			catch (...)
			{
				this->roll_back();
			}
		}
	};

} // namespace elastic