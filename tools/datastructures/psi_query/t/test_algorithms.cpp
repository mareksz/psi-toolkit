#include "tests.hpp"
#include "fsa_dfsa.hpp"
#include "fsa_algorithms.hpp"

BOOST_AUTO_TEST_CASE(algorithms_test)
{
    psi::fsa::DFSA<> fsa;
    psi::fsa::State q0 = fsa.addState();
    psi::fsa::State q1 = fsa.addState();
    psi::fsa::State q2 = fsa.addState();
    psi::fsa::State q3 = fsa.addState();

    fsa.setStartState(q0);
    fsa.setEndState(q3);

    fsa.addArc(q0, psi::fsa::ArcWeighted<>('a', q1));
    fsa.addArc(q0, psi::fsa::ArcWeighted<>('b', q1));
    fsa.addArc(q0, psi::fsa::ArcWeighted<>('c', q1));

    fsa.addArc(q1, psi::fsa::ArcWeighted<>('a', q1));
    fsa.addArc(q1, psi::fsa::ArcWeighted<>('b', q2));
    fsa.addArc(q1, psi::fsa::ArcWeighted<>('c', q3));

    fsa.addArc(q2, psi::fsa::ArcWeighted<>('a', q1));
    fsa.addArc(q2, psi::fsa::ArcWeighted<>('b', q3));
    fsa.addArc(q2, psi::fsa::ArcWeighted<>('c', q3));

    fsa.addArc(q3, psi::fsa::ArcWeighted<>('a', q1));
    fsa.addArc(q3, psi::fsa::ArcWeighted<>('b', q3));
    fsa.addArc(q3, psi::fsa::ArcWeighted<>('c', q3));

    psi::fsa::Reverser<psi::fsa::DFSA<> > reverser(fsa); //Warning signed and unsigned

    //Check the size of sets////////////////////////////////////////////
    BOOST_CHECK_EQUAL(fsa.getEndStates().size(), reverser.startFn().size());
    //BOOST_CHECK_EQUAL(fsa.getStartStates().size(), reverser.finalFn().size());
    ////////////////////////////////////////////////////////////////////

    //Checking reverser start-end states////////////////////////////////
    std::set< psi::fsa::DFSA<>::state_type >::iterator reverserIt;
    std::set< psi::fsa::DFSA<>::state_type >::iterator fsaIt;

    std::set< psi::fsa::DFSA<>::state_type > startfn = reverser.startFn();
    for (reverserIt = startfn.begin(); reverserIt != startfn.end(); ++reverserIt) {
        BOOST_CHECK_EQUAL(fsa.isEndState( *reverserIt ), true);
    }

    //I dont check the size of fsa it should be equal in previous test
    for (reverserIt = startfn.begin(), fsaIt = fsa.getEndStates().begin();
        reverserIt != startfn.end();
        ++reverserIt, ++fsaIt ) {

        BOOST_CHECK_EQUAL( *reverserIt, *fsaIt );
    }

    //End states
    for (fsaIt = fsa.getStartStates().begin();
        fsaIt != fsa.getStartStates().end();
        ++fsaIt) {

        BOOST_CHECK_EQUAL(reverser.finalFn( *fsaIt ), true);
    }

    ////////////////////////////////////////////////////////////////////
}
