#pragma once
#include <elastic/archive/basic_oarchive.hpp>
#include <elastic/archive/interface_oarchive.hpp>
#include <elastic/archive/oserialize.hpp>

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
		void save_override(_Ty&& t)
		{
			vsave();

			archive::save(*this->_this(), std::forward<_Ty>(t));
		}

		void save_start(const char*)
		{}

		void save_end(const char*)
		{}
	};
} // namespace elastic