#pragma once
#include "data.pb.h"
#include "config.hpp"
#include "scope_timer.hpp"

class proto_base
{
public:
	proto_base()
	{
		create();
	}

public:
	void serialize()
	{
		std::cout << "protobuf serialize:\n";
		scoped_timer time("proto_base");

		for (int i = 0; i < PARSE_TIMES; ++i)
		{
			auto str = data_.SerializeAsString();
		}
	}

	void deserialize()
	{
		std::cout << "protobuf deserialize:\n";

		auto str = data_.SerializeAsString();

		scoped_timer time("proto_base");


		for (int i = 0; i < PARSE_TIMES; ++i)
		{
			data_.ParseFromString(str);
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
		data_.set_role_data(std::string("abcde"));
		data_.set_hp(22.90f);
		data_.set_mana(33.98);
		data_.set_input_stream(123);
		data_.set_output_stream(456);
	}

private:
	yyy::data data_;
};