#pragma once
#include "basic_binary_archive.hpp"
#include "binary_primitive.hpp"

namespace elastic
{
	template <typename _Archive, typename _Elem, typename _Traits>
	class binary_oarchive_impl : public binary_oprimitive<_Archive, _Elem, _Traits>,
								 public basic_binary_oarchive<_Archive>
	{
	protected:
		binary_oarchive_impl(std::basic_streambuf<_Elem, _Traits>& os)
			: binary_oprimitive<_Archive, _Elem, _Traits>(os)
			, basic_binary_oarchive<_Archive>()
		{}

		binary_oarchive_impl(std::basic_ostream<_Elem, _Traits>& os)
			: binary_oprimitive<_Archive, _Elem, _Traits>(*os.rdbuf())
			, basic_binary_oarchive<_Archive>()
		{}

	public:
		template <typename _Ty>
		void save_override(_Ty&& t)
		{
			this->basic_binary_oarchive<_Archive>::save_override(std::forward<_Ty>(t));
		}
	};
} // namespace elastic