#include "test_proto.els.h

namespace xxx
{
	const int32_t& person::age()
	{
		return impl.age;
	}

	const int32_t& person::age() const
	{
		return impl.age;
	}

	void person::set_age(const int32_t& age)
	{
		impl.age = age;
	}

	const std::string& person::name()
	{
		return impl.name;
	}

	const std::string& person::name() const
	{
		return impl.name;
	}

	void person::set_name(const std::string& name)
	{
		impl.name = name;
	}

	const bool& person::sex()
	{
		return impl.sex;
	}

	const bool& person::sex() const
	{
		return impl.sex;
	}

	void person::set_sex(const bool& sex)
	{
		impl.sex = sex;
	}

	const int64_t& person::money()
	{
		return impl.money;
	}

	const int64_t& person::money() const
	{
		return impl.money;
	}

	void person::set_money(const int64_t& money)
	{
		impl.money = money;
	}

	const uint32_t& person::back_money()
	{
		return impl.back_money;
	}

	const uint32_t& person::back_money() const
	{
		return impl.back_money;
	}

	void person::set_back_money(const uint32_t& back_money)
	{
		impl.back_money = back_money;
	}

	const uint64_t& person::crc()
	{
		return impl.crc;
	}

	const uint64_t& person::crc() const
	{
		return impl.crc;
	}

	void person::set_crc(const uint64_t& crc)
	{
		impl.crc = crc;
	}

	const std::vector<uint8_t>& person::role_data()
	{
		return impl.role_data;
	}

	const std::vector<uint8_t>& person::role_data() const
	{
		return impl.role_data;
	}

	void person::set_role_data(const std::vector<uint8_t>& role_data)
	{
		impl.role_data = role_data;
	}

	const float& person::hp()
	{
		return impl.hp;
	}

	const float& person::hp() const
	{
		return impl.hp;
	}

	void person::set_hp(const float& hp)
	{
		impl.hp = hp;
	}

	const double& person::mana()
	{
		return impl.mana;
	}

	const double& person::mana() const
	{
		return impl.mana;
	}

	void person::set_mana(const double& mana)
	{
		impl.mana = mana;
	}

	const elastic::fixed<uint32_t>& person::input_stream()
	{
		return impl.input_stream;
	}

	const elastic::fixed<uint32_t>& person::input_stream() const
	{
		return impl.input_stream;
	}

	void person::set_input_stream(const elastic::fixed<uint32_t>& input_stream)
	{
		impl.input_stream = input_stream;
	}

	const elastic::fixed<uint64_t>& person::output_stream()
	{
		return impl.output_stream;
	}

	const elastic::fixed<uint64_t>& person::output_stream() const
	{
		return impl.output_stream;
	}

	void person::set_output_stream(const elastic::fixed<uint64_t>& output_stream)
	{
		impl.output_stream = output_stream;
	}

	const std::map<uint32_t,std::string>& person::pairs()
	{
		return impl.pairs;
	}

	const std::map<uint32_t,std::string>& person::pairs() const
	{
		return impl.pairs;
	}

	void person::set_pairs(const std::map<uint32_t,std::string>& pairs)
	{
		impl.pairs = pairs;
	}
}