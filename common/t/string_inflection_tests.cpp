#include "tests.hpp"
#include "config.hpp"

#include "string_inflection.hpp"

BOOST_AUTO_TEST_SUITE( string_inflection )

BOOST_AUTO_TEST_CASE( pluralize_regular_word ) {
    BOOST_CHECK_EQUAL(pluralizeRegularWord("party"), "parties");

    BOOST_CHECK_EQUAL(pluralizeRegularWord("crisis"), "crises");
    BOOST_CHECK_EQUAL(pluralizeRegularWord("oasis"), "oases");

    BOOST_CHECK_EQUAL(pluralizeRegularWord("box"), "boxes");
    BOOST_CHECK_EQUAL(pluralizeRegularWord("boss"), "bosses");
    BOOST_CHECK_EQUAL(pluralizeRegularWord("bush"), "bushes");
    BOOST_CHECK_EQUAL(pluralizeRegularWord("church"), "churches");

    BOOST_CHECK_EQUAL(pluralizeRegularWord("quiz"), "quizzes");

    BOOST_CHECK_EQUAL(pluralizeRegularWord("woman"), "women");

    BOOST_CHECK_EQUAL(pluralizeRegularWord("bag"), "bags");
    BOOST_CHECK_EQUAL(pluralizeRegularWord("minute"), "minutes");
}

BOOST_AUTO_TEST_SUITE_END()
