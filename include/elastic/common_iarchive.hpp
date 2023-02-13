#pragma once
#include <elastic/interface_iarchive.hpp>
#include <elastic/basic_iarchive.hpp>
#include <elastic/iserialize.hpp>

namespace elastic
{
	template<typename _Archive>
	class common_iarchive : public basic_iarchive, public interface_iarchive<_Archive>
	{
	protected:
		common_iarchive(uint32_t flag =0)
			: basic_iarchive(flag)
			, interface_iarchive<_Archive>()
		{

		}

	protected:
		template<typename _Ty>
		void load_override(_Ty& t)
		{
			archive::load(*this->_this(), t);
		}

		void load_start()
		{

		}
		
		void laod_end()
		{

		}
	};
}
