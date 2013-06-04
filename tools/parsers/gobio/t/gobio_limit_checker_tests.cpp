#include "tests.hpp"

#include <boost/scoped_ptr.hpp>

#include "gobio_limit_checker.hpp"

BOOST_AUTO_TEST_SUITE( gobio_limit_checker )

BOOST_AUTO_TEST_CASE( pl_limit_checker ) {
    boost::scoped_ptr<GobioLimitChecker> checker(new GobioLimitChecker("pl"));
    BOOST_CHECK(checker->isAllowed(0, 0));
    BOOST_CHECK(checker->isAllowed(0, -999));
    BOOST_CHECK(checker->isAllowed(-999, 9999));
    BOOST_CHECK(checker->isAllowed(0, 9999));
    BOOST_CHECK(checker->isAllowed(20, 9999));
    BOOST_CHECK(!checker->isAllowed(21, 9999));
    BOOST_CHECK(!checker->isAllowed(21, 51));
    BOOST_CHECK(checker->isAllowed(21, 50));
    BOOST_CHECK(checker->isAllowed(30, 50));
    BOOST_CHECK(!checker->isAllowed(31, 50));
    BOOST_CHECK(!checker->isAllowed(31, 21));
    BOOST_CHECK(checker->isAllowed(31, 20));
    BOOST_CHECK(checker->isAllowed(40, 20));
    BOOST_CHECK(!checker->isAllowed(41, 20));
    BOOST_CHECK(!checker->isAllowed(41, 13));
    BOOST_CHECK(checker->isAllowed(41, 12));
    BOOST_CHECK(checker->isAllowed(100, 12));
    BOOST_CHECK(!checker->isAllowed(101, 12));
    BOOST_CHECK(!checker->isAllowed(101, 9));
    BOOST_CHECK(checker->isAllowed(101, 8));
    BOOST_CHECK(checker->isAllowed(9999, 8));
    BOOST_CHECK(checker->isAllowed(9999, 0));
    BOOST_CHECK(checker->isAllowed(9999, -999));
    BOOST_CHECK(!checker->isAllowed(9999, 9999));
}

BOOST_AUTO_TEST_SUITE_END()
