#pragma once
#include <elastic/common_iarchive.hpp>

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

		void load_override(tracking_type& t, int /*version*/)
		{
			//boost::serialization::library_version_type lv = this->get_library_version();
			//if (boost::serialization::library_version_type(6) < lv)
			//{
			//	int_least8_t x = 0;
			//	*this->This() >> x;
			//	t = tracking_type(x);
			//}
			//else
			//{
			//	bool x = 0;
			//	*this->This() >> x;
			//	t = tracking_type(x);
			//}
		}
		void load_override(class_id_type& t)
		{
			//boost::serialization::library_version_type lv = this->get_library_version();
			///*
			// * library versions:
			// *   boost 1.39 -> 5
			// *   boost 1.43 -> 7
			// *   boost 1.47 -> 9
			// *
			// *
			// * 1) in boost 1.43 and inferior, class_id_type is always a 16bit value, with no check on the library
			// * version
			// *   --> this means all archives with version v <= 7 are written with a 16bit class_id_type
			// * 2) in boost 1.44 this load_override has disappeared (and thus boost 1.44 is not backward compatible at
			// * all !!) 3) recent boosts reintroduced load_override with a test on the version :
			// *     - v > 7 : this->detail_common_iarchive::load_override(t, version)
			// *     - v > 6 : 16bit
			// *     - other : 32bit
			// *   --> which is obviously incorrect, see point 1
			// *
			// * the fix here decodes class_id_type on 16bit for all v <= 7, which seems to be the correct behaviour ...
			// */
			//if (boost::serialization::library_version_type(7) < lv)
			//{
			//	this->detail_common_iarchive::load_override(t);
			//}
			//else
			//{
			//	int_least16_t x = 0;
			//	*this->This() >> x;
			//	t = boost::archive::class_id_type(x);
			//}
		}

		void load_override(object_id_type& t)
		{

		}

		void load_override(version_type& t)
		{
			//library_version_type lv = this->get_library_version();
			//if (boost::serialization::library_version_type(7) < lv)
			//{
			//	this->detail_common_iarchive::load_override(t);
			//}
			//else if (boost::serialization::library_version_type(6) < lv)
			//{
			//	uint_least8_t x = 0;
			//	*this->This() >> x;
			//	t = boost::archive::version_type(x);
			//}
			//else if (boost::serialization::library_version_type(5) < lv)
			//{
			//	uint_least16_t x = 0;
			//	*this->This() >> x;
			//	t = boost::archive::version_type(x);
			//}
			//else if (boost::serialization::library_version_type(2) < lv)
			//{
			//	// upto 255 versions
			//	unsigned char x = 0;
			//	*this->This() >> x;
			//	t = version_type(x);
			//}
			//else
			{
				unsigned int x = 0;
				*this->_this() >> x;
				//t = version_type(x);
			}
		}

		void load_override(class_name_type& t)
		{

		}

		void init()
		{

		}
	};
}