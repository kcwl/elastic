#pragma once

namespace elastic
{
	class nocopyable
	{
	public:
		nocopyable(const nocopyable&) = delete;
		nocopyable& operator=(const nocopyable&) = delete;
	};
} // namespace elastic