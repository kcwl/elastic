#pragma once
#include <streambuf>

namespace elastic
{
	template <typename _Elem, typename _Traits>
	class streambuf_guard
	{
	public:
		streambuf_guard(std::basic_streambuf<_Elem, _Traits>& s)
			: streambuf_(s)
			, locale_(s.getloc())
		{}

		~streambuf_guard()
		{
			streambuf_.pubsync();
			streambuf_.pubimbue(locale_);
		}

	private:
		std::basic_streambuf<_Elem, _Traits> streambuf_;
		std::locale locale_;
	};
} // namespace elastic