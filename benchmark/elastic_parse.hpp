#pragma once
#include "config.hpp"
#include "test_proto.els.h"
#include "scope_timer.hpp"

class elastic_base
{
public:
	elastic_base()
		: buffer(100*PARSE_TIMES)
	{
		create();
	}

public:
	void serialize()
	{
		std::cout << "elastic serialize:\n";

		scoped_timer time("person");

		for (int i = 0; i < PARSE_TIMES; ++i)
		{
			data_.to_binary(buffer);
		}
	}

	void deserialize()
	{
		std::cout << "elastic deserialize:\n";

		scoped_timer time("person");

		for (int i = 0; i < PARSE_TIMES; ++i)
		{
			data_.from_binary(buffer);
		}
	}

private:
	void create()
	{
		data_.set_age(1);
		data_.set_name("person");
		data_.set_sex(true);
		data_.set_money(111);
		data_.set_back_money(222);
		data_.set_crc(22222);
		data_.set_role_data(std::vector<uint8_t>{ 'a', 'b', 'c', 'd', 'e' });
		data_.set_hp(22.90f);
		data_.set_mana(33.98);
		data_.set_input_stream(123);
		data_.set_output_stream(456);
	}

private:
	xxx::person data_;

	elastic::flex_buffer_t buffer;
};