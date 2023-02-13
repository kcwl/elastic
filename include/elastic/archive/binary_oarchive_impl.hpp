#pragma once
#include <elastic/archive/basic_binary_oarchive.hpp>
#include <elastic/archive/basic_binary_oprimitive.hpp>

namespace elastic
{
	template <typename _Archive, typename _Elem, typename _Traits>
	class binary_oarchive_impl : public basic_binary_oprimitive<_Archive, _Elem, _Traits>,
								 public basic_binary_oarchive<_Archive>
	{
	protected:
		binary_oarchive_impl(std::basic_streambuf<_Elem, _Traits>& os)
			: basic_binary_oprimitive<_Archive, _Elem, _Traits>(os)
			, basic_binary_oarchive<_Archive>()
		{}

		binary_oarchive_impl(std::basic_ostream<_Elem,_Traits>& os)
			: basic_binary_oprimitive<_Archive, _Elem, _Traits>(*os.rdbuf())
			, basic_binary_oarchive<_Archive>()
		{}

	public:
		template<typename _Ty>
		void save_override(_Ty& t)
		{
			this->basic_binary_oarchive<_Archive>::save_override(t);
		}

		void init()
		{
			this->basic_binary_oarchive<_Archive>::init();
			this->basic_binary_oprimitive<_Archive, _Elem, _Traits>::init();
		}
	};
} // namespace elastic