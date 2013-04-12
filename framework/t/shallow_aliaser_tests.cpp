#include "tests.hpp"

#include <boost/assign/list_of.hpp>
#include "shallow_aliaser.hpp"
#include <iostream>

BOOST_AUTO_TEST_SUITE( shallow_aliaser )

BOOST_AUTO_TEST_CASE( simple ) {
    ShallowAliaser aliaser;

    {
        BOOST_CHECK_EQUAL(aliaser.replace("write-tokens"), "simple-writer --tags token");
    }

    {
        std::vector<std::string> pipeline = boost::assign::list_of("write-tokens");
        std::vector<std::string> result = aliaser.replace(pipeline);

        std::vector<std::string> expected = boost::assign::list_of
            ("simple-writer")("--tags")("token");

        BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(),
                                      expected.begin(), expected.end());
    }
}

BOOST_AUTO_TEST_CASE( more_complex ) {
    ShallowAliaser aliaser;

    {
        BOOST_CHECK_EQUAL(aliaser.replace("read-docx ! tokenize ! write-tokens"),
                          "apertium-reader --format docx ! tokenize ! simple-writer --tags token");
    }
}
BOOST_AUTO_TEST_SUITE_END()
