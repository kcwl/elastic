#pragma once
#include <elastic.hpp>

#include <boost/test/unit_test_suite.hpp>

#include "test_proto.els.h"

BOOST_AUTO_TEST_SUITE(els_tst)

BOOST_AUTO_TEST_CASE(els)
{
	elastic::flex_buffer_t buffer{};

	xxx::person persons{};

	persons.set_age(1);

	persons.set_name("person");

	persons.set_sex(true);

	persons.set_money(1111);

	persons.set_back_money(22222);

	persons.set_crc(111122222);

	persons.set_role_data(std::vector<uint8_t>{ 'a', 'b' });

	persons.set_hp(1.2f);

	persons.set_mana(2222.33);

	persons.set_input_stream(elastic::fixed<uint32_t>(5));

	persons.set_output_stream(elastic::fixed<uint64_t>(10));

	persons.set_pairs(std::map<uint32_t, std::string>{ { 1, "hello" }, { 2, "world" } });

	persons.to_binary(buffer);

	xxx::person per{};

	per.from_binary(buffer);

	BOOST_CHECK(per == persons);

	constexpr bool res = std::is_pod_v<xxx::person>;
}

BOOST_AUTO_TEST_SUITE_END()