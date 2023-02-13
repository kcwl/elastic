#pragma once
#include <elastic/common_iarchive.hpp>
#include <elastic/archive_exception.hpp>

namespace elastic
{
	template<typename _Archive>
	class basic_binary_iarchive : public common_iarchive<_Archive>
	{
	protected:
		basic_binary_iarchive(uint32_t flags)
			: common_iarchive<_Archive>(flags)
		{

		}

	protected:
		template<typename _Ty>
		void load_override(_Ty* t)
		{
			this->common_iarchive<_Archive>::load_override(t);
		}

		void init()
		{
			std::string file_signature;

			*this->_this() >> file_signature;

			if (file_signature() != archive_signature())
				throw(archive_exception(archive_exception::exception_code::invalid_signature));
		}
	};
}