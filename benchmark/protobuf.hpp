#pragma once
#include "data.pb.h"
#include "scope_timer.hpp"

#include <format>

class protobuf
{
public:
	protobuf()
	{
		create();
	}

	~protobuf() = default;

public:
	template <parse_type Value>
	void seriliaze()
	{
		std::cout << std::format("[{}] protobuf serialize:\n", parse_type_str[static_cast<int>(Value)]);

		if constexpr (Value == parse_type::vec3)
		{
			scoped_timer("Vec3");

			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				auto str = vec3_.SerializeAsString();
			}
		}
		else if constexpr (Value == parse_type::weapon)
		{
			scoped_timer("Weapon");
			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				auto str = w1_.SerializeAsString();
			}
		}
		else if constexpr (Value == parse_type::monster)
		{
			scoped_timer("Monster");
			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				auto str = m1_.SerializeAsString();
			}
		}
		else if constexpr (Value == parse_type::monsters)
		{
			scoped_timer("Monsters");
			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				auto str = ms_.SerializeAsString();
			}
		}
		else if constexpr (Value == parse_type::rect32)
		{
			scoped_timer("rect32");
			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				auto str = rc32_.SerializeAsString();
			}
		}
		else if constexpr (Value == parse_type::rect32s)
		{
			scoped_timer("rect32s");
			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				auto str = rc32s_.SerializeAsString();
			}
		}
		else if constexpr (Value == parse_type::person)
		{
			scoped_timer("person");
			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				auto str = pp_.SerializeAsString();
			}
		}
		else if constexpr (Value == parse_type::persons)
		{
			scoped_timer("persons");
			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				auto str = pps_.SerializeAsString();
			}
		}
	}

	template <parse_type Value>
	void deseriliaze()
	{
		std::cout << std::format("[{}] protobuf deserialize:\n", parse_type_str[Value]);

		if constexpr (Value == parse_type::vec3)
		{
			auto str = vec3_.SerializeAsString();

			scoped_timer("Vec3");

			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				vec3_.ParseFromString(str);
			}
		}
		else if constexpr (Value == parse_type::weapon)
		{
			auto str = w1_.SerializeAsString();
			scoped_timer("Weapon");
			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				w1_.ParseFromString(str);
			}
		}
		else if constexpr (Value == parse_type::monster)
		{
			auto str = m1_.SerializeAsString();

			scoped_timer("Monster");
			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				w1_.ParseFromString(str);
			}
		}
		else if constexpr (Value == parse_type::monsters)
		{
			auto str = ms_.SerializeAsString();

			scoped_timer("Monsters");

			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				ms_.ParseFromString(str);
			}
		}
		else if constexpr (Value == parse_type::rect32)
		{
			auto str = rc32_.SerializeAsString();
			scoped_timer("rect32");
			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				rc32_.ParseFromString(str);
			}
		}
		else if constexpr (Value == parse_type::rect32s)
		{
			auto str = rc32s_.SerializeAsString();
			scoped_timer("rect32s");
			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				rc32s_.ParseFromString(str);
			}
		}
		else if constexpr (Value == parse_type::person)
		{
			auto str = pp_.SerializeAsString();
			scoped_timer("person");
			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				pp_.ParseFromString(str);
			}
		}
		else if constexpr (Value == parse_type::persons)
		{
			auto str = pps_.SerializeAsString();

			scoped_timer("persons");
			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				pps_.ParseFromString(str);
			}
		}
	}

private:
	void create()
	{
		vec3_.set_x(1.0f);
		vec3_.set_y(2.0f);
		vec3_.set_z(3.0f);

		w1_.set_damage(1);
		w1_.set_name("weapon");

		auto* pos = m1_.mutable_pos();
		pos->set_x(1);
		pos->set_y(2);
		pos->set_z(3);

		m1_.set_mana(11);
		m1_.set_hp(111);
		m1_.set_name("monster");
		auto* vec = m1_.mutable_inventory();
		vec->push_back('h');
		vec->push_back('e');
		vec->push_back('l');
		vec->push_back('l');
		vec->push_back('o');

		m1_.set_color(mygame::Monster_Color_Red);
		auto* vec1 = m1_.mutable_weapons();
		auto vec1_add = vec1->Add();
		vec1_add->set_name("weapon");
		vec1_add->set_damage(1);

		auto* weapon = m1_.mutable_equipped();
		weapon->set_name("weapon");
		weapon->set_damage(1);

		auto* path = m1_.mutable_path()->Add();
		path->set_x(1);
		path->set_y(1);
		path->set_z(1);

		auto p = *path;
		m1_.mutable_path()->Add(std::move(p));
		p = *path;
		m1_.mutable_path()->Add(std::move(p));
		p = *path;
		m1_.mutable_path()->Add(std::move(p));
		p = *path;
		m1_.mutable_path()->Add(std::move(p));

		auto m1 = m1_;
		ms_.mutable_monsters()->Add(std::move(m1));

		m1 = m1_;
		ms_.mutable_monsters()->Add(std::move(m1));

		m1 = m1_;
		ms_.mutable_monsters()->Add(std::move(m1));

		m1 = m1_;
		ms_.mutable_monsters()->Add(std::move(m1));

		rc32_.set_x(1);
		rc32_.set_y(2);
		rc32_.set_height(3);
		rc32_.set_width(4);

		auto rc = rc32_;
		rc32s_.mutable_rect32_list()->Add(std::move(rc));

		rc = rc32_;
		rc32s_.mutable_rect32_list()->Add(std::move(rc));

		rc = rc32_;
		rc32s_.mutable_rect32_list()->Add(std::move(rc));

		rc = rc32_;
		rc32s_.mutable_rect32_list()->Add(std::move(rc));

		pp_.set_age(1);
		pp_.set_id(2);
		pp_.set_name("pp");
		pp_.set_salary(1);

		auto pp = pp_;
		pps_.mutable_person_list()->Add(std::move(pp));

		pp = pp_;
		pps_.mutable_person_list()->Add(std::move(pp));

		pp = pp_;
		pps_.mutable_person_list()->Add(std::move(pp));

		pp = pp_;
		pps_.mutable_person_list()->Add(std::move(pp));
	}

private:
	mygame::Vec3 vec3_;

	mygame::Weapon w1_;

	mygame::Monster m1_;

	mygame::Monsters ms_;

	mygame::rect32 rc32_;

	mygame::rect32s rc32s_;

	mygame::person pp_;

	mygame::persons pps_;
};