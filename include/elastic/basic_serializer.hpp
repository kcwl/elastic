#pragma once
#include <elastic/extended_type_info.hpp>
#include <elastic/nocopyable.hpp>

namespace elastic
{
	class basic_serializer : private nocopyable
	{
	protected:
		explicit basic_serializer([[maybe_unused]]const extended_type_info& eti)
			//: eti_ptr_(&eti)
		{}

	public:
		inline bool operator<(const basic_serializer& rhs) const
		{
			return get_eti() < rhs.get_eti();
		}

		const char* get_debug_info() const
		{
			return eti_ptr_->get_debug_info();
		}

		const extended_type_info& get_eti() const
		{
			return *eti_ptr_;
		}

	private:
		extended_type_info* eti_ptr_;
	};
} // namespace elastic