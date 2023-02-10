#pragma once
#include <elastic/basic_binary_oarchive.hpp>
#include <elastic/basic_binary_oprimitive.hpp>

namespace elastic
{
	template <typename _Archive, typename _Elem, typename _Traits>
	class binary_oarchive_impl : public basic_binary_oprimitive<_Archive, _Elem, _Traits>,
								 public basic_binary_oarchive<_Archive>
	{
	protected:
		binary_oarchive_impl(std::basic_streambuf& os, uint32_t flags)
			: basic_binary_oprimitive<_Archive, _Elem, _Traits>(os, (flags & active_flags::no_header))
			, basic_binary_oarchive<_Archive>(flags)
		{}

		binary_oarchive_impl(std::basic_ostream& os, uint32_t flags)
			: basic_binary_oprimitive<_Archive, _Elem, _Traits>(*os.rdbuf(), (flags & active_flags::no_header))
			, basic_binary_oarchive<_Archive>(flags)
		{}

	protected:
		template<typename _Ty>
		void save_override(_Ty& t)
		{
			this->~basic_binary_oprimitive<_Archive>::save_override(t);
		}

		void init(uint32_t flags)
		{
			if ((flags & active_flags::no_header) != 0)
				return;

			this->basic_binary_oarchive<_Archive>::init();
			this->basic_binary_oprimitive<_Archive, _Elem, _Traits>::init();
		}
	};
} // namespace elastic