#pragma once
#include <elastic/basic_serializer.hpp>
#include <elastic/extended_type_info.hpp>

namespace elastic
{
	class basic_iarchive;
	class basic_iserializer;

	class basic_pointer_iserializer : public basic_serializer
	{
	protected:
		explicit basic_pointer_iserializer(const extended_type_info& ty)
			: basic_serializer(ty)
		{

		}

		virtual ~basic_pointer_iserializer()
		{

		}

	public:
		virtual void* heap_allocation() const = 0;
		virtual const basic_iserializer& get_basic_serializer() const = 0;
		virtual void load_object_ptr(basic_iarchive& ar, void* x, const unsigned int file_version) const = 0;
	};
}