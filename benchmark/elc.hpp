#pragma once
#include "data.hpp"
#include "scope_timer.hpp"
#include <format>

class elc
{
public:
	elc()
	{
		create();
	}

	~elc() = default;

public:
	template<parse_type Value>
	void serialize()
	{
		std::cout << std::format("[{}] els serialize:\n", parse_type_str[static_cast<int>(Value)]);

		std::stringstream ss{};
		// elastic::to_binary(ev3, ss);
		elastic::binary_oarchive oa(ss);

		if constexpr (Value == parse_type::vec3)
		{
			scoped_timer("vec3");

			for (int i = 0; i < PARSE_TIMES; ++i)
			{

				oa << ev3_;
			}
		}
		else if constexpr (Value == parse_type::weapon)
		{
			scoped_timer("weapon");

			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				oa << w2_;
			}
		}
		else if constexpr (Value == parse_type::monster)
		{
			scoped_timer("monster");

			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				oa << m_;
			}
		}
		else if constexpr (Value == parse_type::monsters)
		{
			scoped_timer("monsters");

			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				oa << ms_;
			}
		}
		else if constexpr (Value == parse_type::rect32)
		{
			scoped_timer("rect32");

			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				oa << rc32_;
			}
		}
		else if constexpr (Value == parse_type::rect32s)
		{
			scoped_timer("rect32s");

			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				oa << rc32s_;
			}
		}
		else if constexpr (Value == parse_type::person)
		{
			scoped_timer("person");

			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				oa << pp_;
			}
		}
		else if constexpr (Value == parse_type::persons)
		{
			scoped_timer("persons");

			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				oa << pps_;
			}
		}
	}

	template <parse_type Value>
	void deserialize()
	{
		std::cout << std::format("[{}] elc deserialize:\n", parse_type_str[static_cast<int>(Value)]);

		std::stringstream ss{};
		elastic::binary_oarchive oa(ss);


		if constexpr (Value == parse_type::vec3)
		{
			oa << ev3_;

			auto str = ss.str();

			scoped_timer("vec3");

			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				std::stringstream ss1{};
				ss1 << str;
				elastic::binary_iarchive ia(ss1);
				ia >> ev3_;
			}
		}
		else if constexpr (Value == parse_type::weapon)
		{
			oa << w2_;

			auto str = ss.str();

			scoped_timer("weapon");

			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				std::stringstream ss1{};
				ss1 << str;
				elastic::binary_iarchive ia(ss1);
				ia >> w2_;
			}
		}
		else if constexpr (Value == parse_type::monster)
		{
			oa << m_;

			auto str = ss.str();

			scoped_timer("monster");

			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				std::stringstream ss1{};
				ss1 << str;
				elastic::binary_iarchive ia(ss1);
				ia >> m_;
			}
		}
		else if constexpr (Value == parse_type::monsters)
		{
			oa << ms_;

			auto str = ss.str();

			scoped_timer("monsters");

			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				std::stringstream ss1{};
				ss1 << str;
				elastic::binary_iarchive ia(ss1);
				ia >> ms_;
			}
		}
		else if constexpr (Value == parse_type::rect32)
		{
			oa << rc32_;

			auto str = ss.str();

			scoped_timer("rect32");

			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				std::stringstream ss1{};
				ss1 << str;
				elastic::binary_iarchive ia(ss1);
				ia >> rc32_;
			}
		}
		else if constexpr (Value == parse_type::rect32s)
		{
			oa << rc32s_;

			auto str = ss.str();

			scoped_timer("rect32s");

			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				std::stringstream ss1{};
				ss1 << str;
				elastic::binary_iarchive ia(ss1);
				ia >> rc32s_;
			}
		}
		else if constexpr (Value == parse_type::person)
		{
			oa << pp_;

			auto str = ss.str();

			scoped_timer("person");

			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				std::stringstream ss1{};
				ss1 << str;
				elastic::binary_iarchive ia(ss1);
				ia >> pp_;
			}
		}
		else if constexpr (Value == parse_type::persons)
		{
			oa << pps_;

			auto str = ss.str();

			scoped_timer("persons");

			for (int i = 0; i < PARSE_TIMES; ++i)
			{
				std::stringstream ss1{};
				ss1 << str;
				elastic::binary_iarchive ia(ss1);
				ia >> pps_;
			}
		}
	}

private:
	void create()
	{
		ev3_.x = 1.0f;
		ev3_.y = 2.0f;
		ev3_.z = 3.0f;

		w2_.damage = 1;
		w2_.name = "weapon";

		m_.pos = { 1, 1, 1 };
		m_.mana = 11;
		m_.hp = 111;
		m_.name = "monster";
		m_.inventory.push_back(std::byte('h'));
		m_.inventory.push_back(std::byte('e'));
		m_.inventory.push_back(std::byte('l'));
		m_.inventory.push_back(std::byte('l'));
		m_.inventory.push_back(std::byte('o'));
		m_.co = monster::color::red;
		m_.weapons.push_back(weapon{ "weapon", 1 });
		m_.equipped.damage = 1;
		m_.equipped.name = "weapon";
		m_.path.push_back({ 1, 1, 1 });
		m_.path.push_back({ 1, 1, 1 });
		m_.path.push_back({ 1, 1, 1 });
		m_.path.push_back({ 1, 1, 1 });

		ms_.monsters_.push_back(m_);
		ms_.monsters_.push_back(m_);
		ms_.monsters_.push_back(m_);
		ms_.monsters_.push_back(m_);

		rc32_.x = 1;
		rc32_.y = 2;
		rc32_.height = 3;
		rc32_.width = 4;

		rc32s_.list.push_back(rc32_);
		rc32s_.list.push_back(rc32_);
		rc32s_.list.push_back(rc32_);
		rc32s_.list.push_back(rc32_);

		pp_.age = 1;
		pp_.id = 2;
		pp_.name = "pp";
		pp_.salary = 1;

		pps_.roles.push_back(pp_);
		pps_.roles.push_back(pp_);
		pps_.roles.push_back(pp_);
		pps_.roles.push_back(pp_);
	}

private:
	vec3 ev3_;

	weapon w2_;

	monster m_;

	monsters ms_;

	rect32 rc32_;

	rect32s rc32s_;

	person pp_;

	persons pps_;
};