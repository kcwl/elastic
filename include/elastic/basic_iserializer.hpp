#pragma once
#include <elastic/basic_serializer.hpp>
#include <elastic/extended_type_info.hpp>
#include <elastic/basic_pointer_iserializer.hpp>
#include <elastic/basic_archive.hpp>

namespace elastic
{
	class basic_iserializer : public basic_serializer
	{
	protected:
		explicit basic_iserializer(const extended_type_info& ty)
			: basic_serializer(ty)
		{

		}

		virtual ~basic_iserializer()
		{}

	public:
		bool serialize_as_pointer() const
		{
			return bpis_ptr_;
		}

		void set_bpis(basic_pointer_iserializer* bpis)
		{
			bpis_ptr_ = bpis;
		}

		const basic_pointer_iserializer* get_bpis() const
		{
			return bpis_ptr_;
		}

		virtual void load_object_data(basic_iarchive& ar, void* x, const uint32_t file_version) = 0;
		virtual bool class_info() const = 0;
		virtual bool tracking(const uint32_t) = 0;
		virtual version_type version() = 0;
		virtual bool is_polymorphic() const = 0;
		virtual void destroy(void*) const = 0;

	private:
		basic_pointer_iserializer* bpis_ptr_;
	};
}