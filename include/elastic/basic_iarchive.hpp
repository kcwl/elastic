#pragma once
#include <cstdint>
#include <elastic/nocopyable.hpp>
#include <elastic/basic_iserializer.hpp>
#include <elastic/>

namespace elastic
{
	class basic_iarchive : private nocopyable
	{
	protected:
		basic_iarchive(uint32_t flags)
		{

		}

	public:
		~basic_iarchive()
		{

		}

	public:
		void next_oject_pointer(void* t)
		{

		}

		void register_basic_serializer(const basic_iserializer& bis)
		{

		}

		void load_object(void* t, const basic_iserializer& bis)
		{

		}

		const basic_pointer_iserializer* load_pointer(
			void*& t, const basic_pointer_iserializer* bpis_ptr,
			const basic_pointer_iserializer* (*finder)(const extended_type_info& eti))
		{

		}

		uint32_t get_flags() const
		{

		}

		void reset_object_address(const void* new_address, const void* old_address)
		{

		}

		void delete_created_pointers()
		{

		}
	};
}