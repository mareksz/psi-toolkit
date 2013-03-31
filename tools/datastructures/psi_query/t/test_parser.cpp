#include <string>

#include "tests.hpp"
#include "fsa_ndfsa.hpp"
#include "fsa_algorithms.hpp"
#include "fsa_query_grammers.hpp"

BOOST_AUTO_TEST_SUITE( psi_query )

BOOST_AUTO_TEST_CASE(parser_test)
{
    typedef fsa::CharGrammar<std::string::const_iterator, fsa::NDFSA<> > Grammar;

    std::string regex = "(będę|będziemy|będą) w ab+";
    std::string::const_iterator iter = regex.begin();
    std::string::const_iterator end = regex.end();

    std::string test1 = "będę w abbbbbbb";
    std::string test2 = "będziemy w abbbbbb";
    std::string test3 = "będa w a";
    std::string test4 = "będziemy bbbbbbbb";

    Grammar g;
    fsa::NDFSA<> a;

    bool r = parse(iter, end, g, a);
    if (r && iter == end) {
        fsa::minimize(a);

        a.print();

        BOOST_CHECK_EQUAL(a.in(test1.begin(), test1.end()), 1);
        BOOST_CHECK_EQUAL(a.in(test2.begin(), test2.end()), 1);
        BOOST_CHECK_EQUAL(a.in(test3.begin(), test3.end()), 0);
        BOOST_CHECK_EQUAL(a.in(test4.begin(), test4.end()), 0);

    }
}

BOOST_AUTO_TEST_SUITE_END()
