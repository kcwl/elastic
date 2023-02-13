#pragma once
#include <elastic/basic_oarchive.hpp>
#include <elastic/interface_oarchive.hpp>

namespace elastic
{
	template <typename _Archive>
	class common_oarchive : public basic_oarchive, public interface_oarchive<_Archive>
	{
		friend class interface_oarchive<_Archive>;
		friend class basic_oarchive;

	protected:
		common_oarchive() = default;

	public:
		virtual void vsave() const override
		{}

	protected:
		template <typename _Ty>
		void save_override(_Ty& t)
		{}

		void save_start(const char*)
		{}

		void save_end(const char*)
		{}
	};
} // namespace elastic