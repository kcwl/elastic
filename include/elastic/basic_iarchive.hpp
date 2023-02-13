#pragma once
#include <cstdint>
#include <elastic/nocopyable.hpp>
#include <elastic/basic_iserializer.hpp>
#include <elastic/basic_archive.hpp>

namespace elastic
{
	class basic_iarchive : private nocopyable
	{
	protected:
		basic_iarchive([[maybe_unused]]uint32_t flags)
		{

		}

	public:
		~basic_iarchive()
		{

		}

	public:
		void next_oject_pointer([[maybe_unused]]void* t)
		{

		}

		void register_basic_serializer([[maybe_unused]]const basic_iserializer& bis)
		{

		}

		void load_object([[maybe_unused]]void* t, [[maybe_unused]]const basic_iserializer& bis)
		{

		}

		const basic_pointer_iserializer* load_pointer(
			[[maybe_unused]]void*& t, [[maybe_unused]]const basic_pointer_iserializer* bpis_ptr,
			[[maybe_unused]]const basic_pointer_iserializer* (*finder)(const extended_type_info& eti))
		{

		}

		uint32_t get_flags() const
		{

		}

		void reset_object_address([[maybe_unused]]const void* new_address, [[maybe_unused]]const void* old_address)
		{

		}

		void delete_created_pointers()
		{

		}

	protected:
		virtual void vload(version_type& t) = 0;
		virtual void vload(object_id_type& t) = 0;
		virtual void vload(class_id_type& t) = 0;
		virtual void vload(tracking_type& t) = 0;
		virtual void vload(class_name_type& s) = 0;
	};
}