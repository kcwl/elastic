#include "test_proto.h"

namespace xxx
{
	int32_t person::age() const
	{
		return impl.age;
	}
	
	void person::set_age(const int32_t& age)
	{
		impl.age = age;
	}
	std::string person::name() const
	{
		return impl.name;
	}
	
	void person::set_name(const std::string& name)
	{
		impl.name = name;
	}
	bool person::sex() const
	{
		return impl.sex;
	}
	
	void person::set_sex(const bool& sex)
	{
		impl.sex = sex;
	}
	uint32_t person::back_money() const
	{
		return impl.back_money;
	}
	
	void person::set_back_money(const uint32_t& back_money)
	{
		impl.back_money = back_money;
	}
	uint64_t person::crc() const
	{
		return impl.crc;
	}
	
	void person::set_crc(const uint64_t& crc)
	{
		impl.crc = crc;
	}
	bytes person::role_data() const
	{
		return impl.role_data;
	}
	
	void person::set_role_data(const bytes& role_data)
	{
		impl.role_data = role_data;
	}
	float person::hp() const
	{
		return impl.hp;
	}
	
	void person::set_hp(const float& hp)
	{
		impl.hp = hp;
	}
	double person::mana() const
	{
		return impl.mana;
	}
	
	void person::set_mana(const double& mana)
	{
		impl.mana = mana;
	}
	fixed32_t person::input_stream() const
	{
		return impl.input_stream;
	}
	
	void person::set_input_stream(const fixed32_t& input_stream)
	{
		impl.input_stream = input_stream;
	}
	fixed64_t person::output_stream() const
	{
		return impl.output_stream;
	}
	
	void person::set_output_stream(const fixed64_t& output_stream)
	{
		impl.output_stream = output_stream;
	}
	
	elastic::message_pod& person::internal_type()
	{
		return impl;
	}
}
