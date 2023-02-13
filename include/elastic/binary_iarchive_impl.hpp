#pragma once
#include <elastic/basic_binary_iprimitive.hpp>
#include <elastic/basic_binary_iarchive.hpp>

namespace elastic
{
	template<typename _Archive,typename _Elem, typename _Traits>
	class binary_iarchive_impl : public basic_binary_iprimitive<_Archive, _Elem, _Tratis>, public basic_binary_iarchive<_Archive>
	{
	protected:
		binary_iarchive_impl(std::basic_streambuf<_Elem, _Traits>& bsb, unsigned int flags)
			: basic_binary_iprimitive<_Archive, _Elem, _Traits>(bsb, 0 != (flags & no_codecvt))
			, basic_binary_iarchive<_Archive>(flags)
		{}
		binary_iarchive_impl(std::basic_istream<_Elem, _Traits>& is, unsigned int flags)
			: basic_binary_iprimitive<_Archive, _Elem, _Traits>(*is.rdbuf(), 0 != (flags & no_codecvt))
			, basic_binary_iarchive<_Archive>(flags)
		{}

	protected:
		template<typename _Ty>
		void load_override(_Ty& t)
		{
			this->basic_binary_iarchive<_Archive>::load_override(t);
		}

		void init(unsigned int flags)
		{
			if (0 != (flags & active_flags::no_header))
			{
				return;
			}

			this->basic_binary_iarchive<_Archive>::init();
			this->basic_binary_iprimitive<_Archive, _Elem, _Traits>::init();
		}
	};
}