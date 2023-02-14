#pragma once
#include <elastic/archive/interface_iarchive.hpp>
#include <elastic/archive/basic_iarchive.hpp>
#include <elastic/archive/iserialize.hpp>

namespace elastic
{
	template<typename _Archive>
	class common_iarchive : public basic_iarchive, public interface_iarchive<_Archive>
	{
	protected:
		common_iarchive() = default;

	public:
		virtual void vload() const override
		{}

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
