#pragma once
#include "archive_exception.hpp"
#include "common_archive.hpp"

namespace elastic
{
	template <typename _Archive>
	class basic_binary_iarchive : public common_iarchive<_Archive>
	{
	protected:
		basic_binary_iarchive()
			: common_iarchive<_Archive>()
		{}

	protected:
		template <typename _Ty>
		void load_override(_Ty& t)
		{
			this->common_iarchive<_Archive>::load_override(t);
		}
	};

	template <typename _Archive>
	class basic_binary_oarchive : public common_oarchive<_Archive>
	{
	protected:
		basic_binary_oarchive()
			: common_oarchive<_Archive>()
		{}

	protected:
		template <typename _Ty>
		void save_override(_Ty&& t)
		{
			this->common_oarchive<_Archive>::save_override(std::forward<_Ty>(t));
		}
	};
} // namespace elastic