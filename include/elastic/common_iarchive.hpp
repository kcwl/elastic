#pragma once
#include <elastic/interface_iarchive.hpp>
#include <elastic/basic_iarchive.hpp>

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

		}

		void load_start()
		{}
		
		void laod_end()
		{

		}

	private:
		void vload(version_type& t) override
		{
			*this->_this() >> t;
		}
		void vload(object_id_type& t) override
		{
			*this->_this() >> t;
		}
		void vload(class_id_type& t) override
		{
			*this->_this() >> t;
		}
		void vload(tracking_type& t) override
		{
			*this->_this() >> t;
		}
		void vload(class_name_type& s) override
		{
			*this->_this() >> s;
		}
	};
}
