#pragma once
#include <cstdint>
#include <elastic/nocopyable.hpp>
#include <elastic/basic_archive.hpp>

namespace elastic
{
	class basic_iarchive : public basic_archive
	{
	public:
		basic_iarchive() = default;
		~basic_iarchive() = default;
	};
}