#pragma once
#include <elastic/archive/common_oarchive.hpp>

namespace elastic
{
	template<typename _Archive>
	class basic_binary_oarchive : public common_oarchive<_Archive>
	{
	protected:
		basic_binary_oarchive()
			: common_oarchive<_Archive>()
		{}

	protected:
		template<typename _Ty>
		void save_override(_Ty&& t)
		{
			this->common_oarchive<_Archive>::save_override(std::forward<_Ty>(t));
		}

		void init()
		{
			const std::string file_signature(archive_signature());
			*this->This() << file_signature;
		}
	};
}