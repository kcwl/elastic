#pragma once
#include "../detail/concepts.hpp"
#include "../detail/nocopyable.hpp"

namespace elastic
{
	class basic_archive : public nocopyable
	{};

	class basic_iarchive : public basic_archive
	{
	public:
		basic_iarchive() = default;
		~basic_iarchive() = default;

	public:
		virtual void vload() const = 0;
	};

	class basic_oarchive : public basic_archive
	{
	public:
		basic_oarchive() = default;
		~basic_oarchive() = default;

	public:
		virtual void vsave() const = 0;
	};
} // namespace elastic