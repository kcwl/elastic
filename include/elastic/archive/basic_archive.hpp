#pragma once
#include <elastic/detail/concepts.hpp>
#include <elastic/detail/nocopyable.hpp>

namespace elastic
{
	class basic_archive : public nocopyable
	{};

	constexpr const char* archive_signature()
	{
		return "serialization::archive";
	}
} // namespace elastic