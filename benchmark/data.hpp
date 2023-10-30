#pragma once
#include <elastic.hpp>

struct vec3
{
	float x;
	float y;
	float z;
};

struct weapon
{
	std::string name;
	int32_t damage;

private:
	friend class elastic::access;

	template <typename _Archive>
	void serialize(_Archive& ar)
	{
		ar& name;
		ar& damage;
	}
};

struct monster
{
	vec3 pos;
	int32_t mana;
	int32_t hp;
	std::string name;
	std::vector<std::byte> inventory;
	enum color
	{
		red = 0,
		green = 1,
		blue = 2
	};
	color co;
	std::vector<weapon> weapons;
	weapon equipped;
	std::vector<vec3> path;

private:
	friend class elastic::access;

	template <typename _Archive>
	void serialize(_Archive& ar)
	{
		ar& pos;
		ar& mana;
		ar& hp;
		ar& name;
		ar& inventory;
		ar& co;
		ar& weapons;
		ar& equipped;
		ar& path;
	}
};

struct monsters
{
	std::vector<monster> monsters_;

private:
	friend class elastic::access;

	template <typename _Archive>
	void serialize(_Archive& ar)
	{
		ar& monsters_;
	}
};

struct rect32
{
	int32_t x;
	int32_t y;
	int32_t width;
	int32_t height;
};

struct rect32s
{
	std::vector<rect32> list;

private:
	friend class elastic::access;

	template <typename _Archive>
	void serialize(_Archive& ar)
	{
		ar& list;
	}
};

struct person
{
	int32_t id;
	std::string name;
	int32_t age;
	double salary;

private:
	friend class elastic::access;

	template <typename _Archive>
	void serialize(_Archive& ar)
	{
		ar& id;
		ar& name;
		ar& age;
		ar& salary;
	}
};

struct persons
{
	std::vector<person> roles;

private:
	friend class elastic::access;

	template <typename _Archive>
	void serialize(_Archive& ar)
	{
		ar& roles;
	}
};