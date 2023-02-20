#pragma once
#include <elastic/archive/basic_archive.hpp>
#include <memory>

namespace elastic
{
	class basic_oarchive : public basic_archive
	{
	public:
		basic_oarchive() = default;
		~basic_oarchive() = default;

	public:
		virtual void vsave() const = 0;
	};
} // namespace elastic