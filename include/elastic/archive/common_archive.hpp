#pragma once
#include "basic_archive.hpp"
#include "interface_archive.hpp"
#include "iserialize.hpp"
#include "oserialize.hpp"

namespace elastic
{
	template <typename _Archive>
	class common_iarchive : public basic_iarchive, public interface_iarchive<_Archive>
	{
	protected:
		common_iarchive() = default;

	public:
		virtual void vload() const override
		{}

	protected:
		template <typename _Ty>
		void load_override(_Ty& t)
		{
			vload();

			archive::load(*this->_this(), t);
		}

		void load_start()
		{}

		void laod_end()
		{}
	};

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
