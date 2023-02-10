#pragma once
#include <elastic/common_oarchive.hpp>

namespace elastic
{
	template<typename _Archive>
	class basic_binary_oarchive : public common_oarchive<_Archive>
	{
	protected:
		basic_binary_oarchive(uint32_t flags)
			: common_oarchive<_Archive>(flags)
		{}

	protected:
		template<typename _Ty>
		void save_override(const _Ty& t)
		{
			this->common_oarchive<_Archive>::save_override(t);
		}

		void init()
		{

		}
	};
}