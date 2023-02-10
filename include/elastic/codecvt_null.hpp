#pragma once
#include <codecvt>

namespace elastic
{
	template <typename _Elem>
	class codecvt_null;

	template <>
	class codecvt_null<char> : public std::codecvt<char, char, std::mbstate_t>
	{
	public:
		explicit codecvt_null(std::size_t no_locale_manage = 0)
			: std::codecvt<char, char, std::mbstate_t>(no_locale_manage)
		{}

		~codecvt_null() override
		{}

	private:
		bool do_always_noconv() const throw() override
		{
			return true;
		}
	};
} // namespace elastic