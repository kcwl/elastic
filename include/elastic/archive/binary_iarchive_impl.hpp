#pragma once
#include <elastic/archive/basic_binary_iprimitive.hpp>
#include <elastic/archive/basic_binary_iarchive.hpp>
#include <istream>

namespace elastic
{
	template<typename _Archive,typename _Elem, typename _Traits>
	class binary_iarchive_impl : public basic_binary_iprimitive<_Archive, _Elem, _Traits>, public basic_binary_iarchive<_Archive>
	{
	protected:
		binary_iarchive_impl(std::basic_streambuf<_Elem, _Traits>& bsb)
			: basic_binary_iprimitive<_Archive, _Elem, _Traits>(bsb)
			, basic_binary_iarchive<_Archive>()
		{}
		binary_iarchive_impl(std::basic_istream<_Elem, _Traits>& is)
			: basic_binary_iprimitive<_Archive, _Elem, _Traits>(*is.rdbuf())
			, basic_binary_iarchive<_Archive>()
		{}

	public:
		template<typename _Ty>
		void load_override(_Ty& t)
		{
			this->basic_binary_iarchive<_Archive>::load_override(t);
		}

		void init()
		{
			this->basic_binary_iarchive<_Archive>::init();
			this->basic_binary_iprimitive<_Archive, _Elem, _Traits>::init();
		}
	};
}