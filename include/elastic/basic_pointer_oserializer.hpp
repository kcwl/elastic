#pragma once
#include <elastic/basic_oserializer.hpp>

namespace elastic
{
	class basic_oarchive;
	class basic_oserializer;

	class basic_pointer_oserializer : public basic_serializer
	{
	protected:
		basic_pointer_oserializer(const extended_type_info& type)
			: basic_serializer(type)
		{

		}

		public:
		virtual ~basic_pointer_oserializer()
		{}

		virtual const basic_oserializer& get_basic_serializer() const = 0;

		virtual void save_object_ptr(basic_oarchive& ar, const void* x) const = 0;
	};
}