#pragma once
#include "defines.h"

#include <string>

namespace elastic
{
	template <typename T, std::size_t... I>
	bool spilt(const char elem, T&& t, std::index_sequence<I...>)
	{
		return ((elem != std::get<I>(std::forward<T>(t))) && ...);
	}

	template <typename... Args>
	bool is_spilt(const char elem, Args&&... args)
	{
		return spilt(elem, std::make_tuple(std::forward<Args>(args)...),
									  std::make_index_sequence<sizeof...(args)>{});
	}

	template <typename... Args>
	std::string trip(std::string& str, Args&&... args)
	{
		for (auto iter = str.begin(); iter != str.end();)
		{
			if (is_spilt(*iter, std::forward<Args>(args)...))
				iter++;
			else
				iter = str.erase(iter);
		}

		return str;
	}

} // namespace elastic