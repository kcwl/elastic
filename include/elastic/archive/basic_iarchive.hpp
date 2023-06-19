#pragma once
#include "../detail/nocopyable.hpp"
#include "basic_archive.hpp"

#include <cstdint>

namespace elastic
{
	class basic_iarchive : public basic_archive
	{
	public:
		basic_iarchive() = default;
		~basic_iarchive() = default;

	public:
		virtual void vload() const = 0;
	};
} // namespace elastic