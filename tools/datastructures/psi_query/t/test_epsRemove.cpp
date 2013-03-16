#include "tests.hpp"

#include "fsa_ndfsa.hpp"
#include "fsa_algorithms.hpp"

BOOST_AUTO_TEST_SUITE( psi_query )

BOOST_AUTO_TEST_CASE(epsremove_test)
{
    // create automaton for (a+b+)?

    psi::fsa::NDFSA<> a(psi::fsa::Symbol('a'));
    psi::fsa::NDFSA<> b(psi::fsa::Symbol('b'));

    psi::fsa::kleene_plus(a);
    psi::fsa::kleene_plus(b);
    psi::fsa::concatenate(a, b);
    psi::fsa::kleene_option(a);

    // remove epsilon transitions

    a.print();
    std::cout << std::endl;

    psi::fsa::epsRemove(a);

    a.print();

    // check for strings

    std::string test1 = "aabb";
    std::string test2 = "aaaabbbbbb";
    std::string test3 = "aaaa";
    std::string test4 = "bbbb";

    BOOST_CHECK_EQUAL(a.in(test1.begin(), test1.end()), 1);
    BOOST_CHECK_EQUAL(a.in(test2.begin(), test2.end()), 1);
    BOOST_CHECK_EQUAL(a.in(test3.begin(), test3.end()), 0);
    BOOST_CHECK_EQUAL(a.in(test4.begin(), test4.end()), 0);
}

BOOST_AUTO_TEST_SUITE_END()
