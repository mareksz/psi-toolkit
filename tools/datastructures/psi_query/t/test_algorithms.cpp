#include "tests.hpp"
#include "fsa_dfsa.hpp"
#include "fsa_algorithms.hpp"

BOOST_AUTO_TEST_CASE(algorithms_test)
{
    fsa::DFSA<> fsa;
    fsa::State q0 = fsa.addState();
    fsa::State q1 = fsa.addState();
    fsa::State q2 = fsa.addState();
    fsa::State q3 = fsa.addState();

    fsa.setStartState(q0);
    fsa.setEndState(q3);

    fsa.addArc(q0, fsa::ArcWeighted<>('a', q1));
    fsa.addArc(q0, fsa::ArcWeighted<>('b', q1));
    fsa.addArc(q0, fsa::ArcWeighted<>('c', q1));

    fsa.addArc(q1, fsa::ArcWeighted<>('a', q1));
    fsa.addArc(q1, fsa::ArcWeighted<>('b', q2));
    fsa.addArc(q1, fsa::ArcWeighted<>('c', q3));

    fsa.addArc(q2, fsa::ArcWeighted<>('a', q1));
    fsa.addArc(q2, fsa::ArcWeighted<>('b', q3));
    fsa.addArc(q2, fsa::ArcWeighted<>('c', q3));

    fsa.addArc(q3, fsa::ArcWeighted<>('a', q1));
    fsa.addArc(q3, fsa::ArcWeighted<>('b', q3));
    fsa.addArc(q3, fsa::ArcWeighted<>('c', q3));

    fsa::Reverser<fsa::DFSA<> > reverser(fsa); //Warning signed and unsigned

    //Check the size of sets////////////////////////////////////////////
    BOOST_CHECK_EQUAL(fsa.getEndStates().size(), reverser.startFn().size());
    //BOOST_CHECK_EQUAL(fsa.getStartStates().size(), reverser.finalFn().size());
    ////////////////////////////////////////////////////////////////////

    //Checking reverser start-end states////////////////////////////////
    std::set< fsa::DFSA<>::state_type >::iterator reverserIt;
    std::set< fsa::DFSA<>::state_type >::iterator fsaIt;

    std::set< fsa::DFSA<>::state_type > startfn = reverser.startFn();
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
