#pragma once
#include <boost/test/unit_test_suite.hpp>
#include "../include/elastic/archive/archive_exception.hpp"

BOOST_AUTO_TEST_SUITE(exception)

BOOST_AUTO_TEST_CASE(archive_exception_test)
{
	elastic::archive_exception exc(elastic::archive_exception::exception_number::input_stream_error, "test", "test");
	exc.what();

	elastic::archive_exception exc1(exc);

	elastic::archive_exception exc2(elastic::archive_exception::exception_number::output_stream_error, "test", "test");
}

BOOST_AUTO_TEST_SUITE_END()