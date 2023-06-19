#pragma once
#include "../detail/concepts.hpp"
#include "../detail/nocopyable.hpp"

namespace elastic
{
	class basic_archive : public nocopyable
	{};

	constexpr const char* archive_signature()
	{
		return "serialization::archive";
	}
} // namespace elastic