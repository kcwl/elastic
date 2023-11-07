#include "test_proto.els.h"

namespace xxx
{
	const int32_t& age()
	{
		return impl.age;
	}

	const int32_t& age() const
	{
		return impl.age;
	}

	void set_age(const int32_t& age)
	{
		impl.age = age;
	}

	const std::string& name()
	{
		return impl.name;
	}

	const std::string& name() const
	{
		return impl.name;
	}

	void set_name(const std::string& name)
	{
		impl.name = name;
	}

	const bool& sex()
	{
		return impl.sex;
	}

	const bool& sex() const
	{
		return impl.sex;
	}

	void set_sex(const bool& sex)
	{
		impl.sex = sex;
	}

	const int64_t& money()
	{
		return impl.money;
	}

	const int64_t& money() const
	{
		return impl.money;
	}

	void set_money(const int64_t& money)
	{
		impl.money = money;
	}

	const uint32_t& back_money()
	{
		return impl.back_money;
	}

	const uint32_t& back_money() const
	{
		return impl.back_money;
	}

	void set_back_money(const uint32_t& back_money)
	{
		impl.back_money = back_money;
	}

	const uint64_t& crc()
	{
		return impl.crc;
	}

	const uint64_t& crc() const
	{
		return impl.crc;
	}

	void set_crc(const uint64_t& crc)
	{
		impl.crc = crc;
	}

	const std::vector<uint8_t>& role_data()
	{
		return impl.role_data;
	}

	const std::vector<uint8_t>& role_data() const
	{
		return impl.role_data;
	}

	void set_role_data(const std::vector<uint8_t>& role_data)
	{
		impl.role_data = role_data;
	}

	const float& hp()
	{
		return impl.hp;
	}

	const float& hp() const
	{
		return impl.hp;
	}

	void set_hp(const float& hp)
	{
		impl.hp = hp;
	}

	const double& mana()
	{
		return impl.mana;
	}

	const double& mana() const
	{
		return impl.mana;
	}

	void set_mana(const double& mana)
	{
		impl.mana = mana;
	}

	const elastic::fixed<uint32_t>& input_stream()
	{
		return impl.input_stream;
	}

	const elastic::fixed<uint32_t>& input_stream() const
	{
		return impl.input_stream;
	}

	void set_input_stream(const elastic::fixed<uint32_t>& input_stream)
	{
		impl.input_stream = input_stream;
	}

	const elastic::fixed<uint64_t>& output_stream()
	{
		return impl.output_stream;
	}

	const elastic::fixed<uint64_t>& output_stream() const
	{
		return impl.output_stream;
	}

	void set_output_stream(const elastic::fixed<uint64_t>& output_stream)
	{
		impl.output_stream = output_stream;
	}

}