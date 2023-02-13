#pragma once
#include <elastic/detail/concepts.hpp>
#include <elastic/nocopyable.hpp>

namespace elastic
{
	class basic_archive : public nocopyable
	{};

	const char* archive_signature()
	{
		return "serialization::archive";
	}
} // namespace elastic