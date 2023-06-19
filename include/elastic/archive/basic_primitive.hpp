#pragma once
#include "../detail/concepts.hpp"

namespace elastic
{
	template <typename _Elem, typename _Traits>
	class basic_primitive
	{
	public:
		explicit basic_primitive(std::basic_streambuf<_Elem, _Traits>& bs)
			: streambuf_(bs)
		{}

	protected:
		std::basic_streambuf<_Elem, _Traits>& streambuf_;
	};
} // namespace elastic