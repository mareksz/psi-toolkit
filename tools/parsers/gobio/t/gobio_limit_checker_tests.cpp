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
    BOOST_CHECK(checker->isAllowed(9999, 0));
    BOOST_CHECK(checker->isAllowed(9999, -999));
    BOOST_CHECK(!checker->isAllowed(9999, 9999));
}

BOOST_AUTO_TEST_SUITE_END()
