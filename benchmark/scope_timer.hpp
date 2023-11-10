#pragma once
#include <chrono>
#include <iostream>
#include "config.hpp"

class scoped_timer
{
public:
	scoped_timer(const char* name)
		: m_name(name)
		, m_beg(std::chrono::high_resolution_clock::now())
	{}
	scoped_timer(const char* name, uint64_t& ns)
		: scoped_timer(name)
	{
		m_ns = &ns;
	}
	~scoped_timer()
	{
		auto end = std::chrono::high_resolution_clock::now();
		auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(end - m_beg);
		auto dur_s = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_beg);
		if (m_ns)
			*m_ns = dur.count();

		std::cout << m_name << " :     "   << dur.count() << " ns\n";
		std::cout << m_name << " :     " << dur_s.count() << " ms\n";
	}

private:
	const char* m_name;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_beg;
	uint64_t* m_ns = nullptr;
};