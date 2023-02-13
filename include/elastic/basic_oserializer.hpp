#pragma once
#include <elastic/basic_serializer.hpp>
#include <elastic/basic_archive.hpp>

namespace elastic
{
	class basic_pointer_oserializer;
	class basic_oarchive;

	class basic_oserializer : public basic_serializer
	{
	protected:
		basic_oserializer(const extended_type_info& type)
			: basic_serializer(type)
		{}

		virtual ~basic_oserializer() = default;

	public:
		bool serialized_as_pointer()const
		{
			return bpos_ptr_ != nullptr;
		}

		void set_bpos(basic_pointer_oserializer* bpos)
		{
			bpos_ptr_ = bpos;
		}

		const basic_pointer_oserializer* get_bpos()
		{
			return bpos_ptr_;
		}

		virtual void save_object_data(basic_oarchive& ar, const void* x) const = 0;

		virtual bool class_info() const = 0;

		virtual bool tracking(const uint32_t flags) const = 0;

		virtual version_type version() = 0;

		virtual bool is_polymorphic() const = 0;

	private:
		basic_pointer_oserializer* bpos_ptr_;
	};
}