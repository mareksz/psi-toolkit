#include "tests.hpp"
#include "config.hpp"

#include "edit_distance.hpp"

BOOST_AUTO_TEST_SUITE( edit_distance )

BOOST_AUTO_TEST_CASE( levenshtein_distance_typical_usage ) {
    std::string word1 = "tokenizer";
    std::string word2 = "tokenizre";

    BOOST_CHECK_EQUAL(levenshteinDistance(word1, word2), 2);
    BOOST_CHECK_EQUAL(levenshteinDistance(word1, word2), levenshteinDistance(word2, word1));

    BOOST_CHECK_EQUAL(levenshteinDistance(word1, word1), 0);

    BOOST_CHECK_EQUAL(levenshteinDistance(word1, std::string("")), word1.length());
}

BOOST_AUTO_TEST_SUITE_END()
