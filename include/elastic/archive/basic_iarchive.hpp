#pragma once
#include <cstdint>
#include <elastic/detail/nocopyable.hpp>
#include <elastic/archive/basic_archive.hpp>

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
}