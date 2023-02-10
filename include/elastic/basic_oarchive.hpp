#pragma once
#include <elastic/nocopyable.hpp>
#include <memory>
#include <elastic/basic_archive.hpp>
#include <elastic/basic_oserializer.hpp>

namespace elastic
{
	class basic_oarchive_impl;

	class basic_oarchive : public nocopyable
	{
	protected:
		basic_oarchive(uint32_t flags = 0)
			: impl_ptr_(new basic_oarchive_impl(flags))
		{

		}

		~basic_oarchive() = default;

	public:
		void register_basic_serializer(const basic_oserializer& bos)
		{
			impl_ptr_->register_type(bos);
		}

		void save_object(const void* x, const basic_oserializer& bos)
		{
			return impl_ptr_->save_object(x, bos);
		}

		void save_pointer(const void* t, const basic_pointer_oserializer* bpos_ptr)
		{
			return impl_ptr_->save_pointer(t, bpos_ptr);
		}

		void end_preamble()
		{

		}

		uint32_t get_flags()
		{
			return impl_ptr_->get_flags();
		}

	private:
		virtual void vsave(const version_type) = 0;
		virtual void vsave(const object_id_type) = 0;
		virtual void vsave(const class_id_type) = 0;
		virtual void vsave(const class_name_type) = 0;
		virtual void vsave(const tracking_type) = 0;

	private:
		friend class basic_oarchive_impl;
		std::unique_ptr<basic_oarchive_impl> impl_ptr_;
	};
}