#ifndef ALGORITHMS_HDR
#define ALGORITHMS_HDR

#include <map>
#include <vector>
#include <set>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>

#include "logging.hpp"

#include "fsa_types.hpp"

/*******************************************************************************
# Algorithms

Algorithms for manipulation of all finite state automaton types included in the
library. Processing is "in-place", the first argument is destroyed and
replaced by the resulting automaton. Therefore applying a function to a finite
state automaton invalidates all previously used iterators and member objects.

## Synopsis

Creating an automaton that accepts the following regular expression: `(a|b|c)+aabb(cc)?`

NDFSA<> fsa;

NDFSA<> tfsa1("a");
NDFSA<> tfsa2("b");
NDFSA<> tfsa3("c");

unify(fsa, tfsa1);
unify(fsa, tfsa2);
unify(fsa, tfsa3);

kleene_plus(fsa);

NDFSA<> tfsa4("aabb");

concatenate(fsa, tfsa4);

NDFSA<> tfsa5("cc");
kleene_option(tfsa5);

concatenate(fsa, tfsa5);

minimize(fsa);

std::cout << fsa.in("aabcccaaaabbcc") << std::endl; // prints 1
std::cout << fsa.in("aabcccaaaabb") << std::endl; // prints 1
std::cout << fsa.in("aabcccaaaabcc") << std::endl; // prints 0


*******************************************************************************/


namespace fsa {

/*******************************************************************************
## Traverse

template <typename Wrapper, typename FSA>
void traverse(Wrapper &wrapper, FSA &fsa);

Traverses the finite state automaton dynamically generated by the wrapper object
of type Wrapper and creates a static copy of type FSA. The wrapper object has to
provide several methods and member types (see below). Most operation on finite
state automatons are performed using this function and an appropriate wrapper
object.

*******************************************************************************/
template <typename Wrapper, typename FSA>
void traverse(Wrapper &wrapper, FSA &fsa) {
    typedef typename Wrapper::state_type WrapperState;
    typedef typename Wrapper::arc_type WrapperArc;

    typedef typename FSA::state_type State;
    typedef typename FSA::arc_type Arc;

    std::map<WrapperState, State> mapper;
    std::vector<WrapperState> queue;

    BOOST_FOREACH( WrapperState s, wrapper.startFn() ) {
        State n = fsa.addState(true);
        mapper[s] = n;
        queue.push_back(s);
    }

    while (!queue.empty()) {
        WrapperState s = queue.back();
        queue.pop_back();

        if (!mapper.count(s)) {
            State n = fsa.addState();
            mapper[s] = n;
        }

        if (wrapper.finalFn(s))
            fsa.setEndState(mapper[s]);

        BOOST_FOREACH( WrapperArc arc, wrapper.arcFn(s) ) {
            if (!mapper.count(arc.getDest())) {
                State n = fsa.addState();
                mapper[arc.getDest()] = n;
                queue.push_back(arc.getDest());
            }
            fsa.addArc(
                mapper[s], Arc(arc.getSymbol(), mapper[arc.getDest()])
            );
        }
    }
}


/*******************************************************************************
## Wrapper object: Reverser

template <typename FSA> class Reverser;

A wrapper object that provides are dynamically generated reversed finite-state
automaton. The public members and types of this wrapper object are common for all
wrappers.

*******************************************************************************/
template <typename FSA>
class Reverser {
  public:
    typedef typename FSA::state_type state_type;
    typedef typename FSA::arc_type arc_type;
    typedef typename FSA::arc_iterator_type arc_iterator_type;

    Reverser(const FSA &fsa) : m_fsa(fsa) {
        for (state_type p = 0; (size_t)p < m_fsa.size(); p++) {
            ArcRange<arc_iterator_type> arcRange = m_fsa.getArcs(p);
            for (arc_iterator_type ait = arcRange.first; ait != arcRange.second; ait++) {
                state_type temp = ait->getDest();
                arc_type arc(*ait);
                arc.setDest(p);
                //std::cerr << "r: " << temp << std::endl;
                reversedArcs[temp].insert(arc);
            }
        }
    }

    std::set<state_type> startFn() const {
        return m_fsa.getEndStates();
    }

    bool finalFn(state_type p) const {
        return m_fsa.isStartState(p);
    }

    std::set<arc_type, ArcSorter> arcFn(state_type p) const {
        //std::cerr << "revarcs: " << reversedArcs.size() << " " << p << std::endl;
        typename std::map<state_type, std::set<arc_type, ArcSorter> >
            ::const_iterator cit = reversedArcs.find(p);
        if (cit != reversedArcs.end())
            return cit->second;
        else
            return std::set<arc_type, ArcSorter>();
    }

  private:
    const FSA &m_fsa;
    std::map<state_type, std::set<arc_type, ArcSorter> > reversedArcs;
};

/*******************************************************************************
## Wrapper object: Copier

template <typename FSA> class Copier;

A wrapper object that provides a dynamically generated finite-state that is an
unaltered copy of the wrapped static finite-state automaton. The public members
and types of this wrapper object are common for all wrappers.

*******************************************************************************/
template <typename FSA>
class Copier {
  public:
    typedef typename FSA::state_type state_type;
    typedef typename FSA::arc_type arc_type;
    typedef typename FSA::arc_iterator_type arc_iterator_type;

    Copier(const FSA &fsa) : m_fsa(fsa) {}

    std::set<state_type> startFn() const {
        return m_fsa.getStartStates();
    }

    bool finalFn(state_type p) const {
        return m_fsa.isEndState(p);
    }

    std::set<arc_type, ArcSorter> arcFn(state_type p) const {
        std::set<arc_type, ArcSorter> arcs;
        ArcRange<arc_iterator_type> arcRange = m_fsa.getArcs(p);
        for (arc_iterator_type ait = arcRange.first; ait != arcRange.second; ait++)
            arcs.insert(*ait);
        return arcs;
    }

  private:
    const FSA &m_fsa;
};

/*******************************************************************************
## Wrapper object: Intersector

template <typename FSA> class Intersector;

A wrapper object that provides a dynamically generated finite-state automaton that
is the intersection of the two finite-state automatons provided in the constructor.
The public members and types of this wrapper object are common for all wrappers.

*******************************************************************************/
template <typename FSA1, typename FSA2>
class Intersector {
  public:
    typedef std::pair<typename FSA1::state_type, typename FSA2::state_type> state_type;
    typedef Arc<typename FSA1::arc_type::symbol_type, state_type> arc_type;

    Intersector(const FSA1 &fsa1, const FSA2 &fsa2, bool allowAny = false)
      : m_fsa1(fsa1), m_fsa2(fsa2), m_allowAny(allowAny) {}

    std::set<state_type> startFn() const {
        std::set<state_type> starts;
        BOOST_FOREACH( typename FSA1::state_type p1, m_fsa1.getStartStates() )
            BOOST_FOREACH( typename FSA2::state_type p2, m_fsa1.getStartStates() )
                starts.insert(state_type(p1, p2));

        return starts;
    }

    bool finalFn(state_type p) const {
        return (m_fsa1.isEndState(p.first) && m_fsa2.isEndState(p.second));
    }

    std::set<arc_type, ArcSorter> arcFn(state_type p) const {
        std::set<arc_type, ArcSorter> arcs;

        BOOST_FOREACH(typename FSA1::state_type p1, m_fsa1.epsClosure(p.first)) {
            BOOST_FOREACH(typename FSA2::state_type p2, m_fsa2.epsClosure(p.second)) {
                ArcRange<typename FSA1::arc_iterator_type> arcRange1 = m_fsa1.getArcs(p1);
                ArcRange<typename FSA2::arc_iterator_type> arcRange2 = m_fsa2.getArcs(p2);

                for (typename FSA1::arc_iterator_type ait1 = arcRange1.first;
                    ait1 != arcRange1.second; ait1++) {
                    for (typename FSA2::arc_iterator_type ait2 = arcRange2.first;
                        ait2 != arcRange2.second; ait2++) {
                        if (ait1->getSymbol() == ait2->getSymbol()) {
                           arcs.insert(
                               arc_type(
                                   ait1->getSymbol(),
                                   state_type(ait1->getDest(), ait2->getDest())
                               )
                           );
                        }

                        if (m_allowAny) {
                            if (ait1->getSymbol() == ANY) {
                                arcs.insert(
                                    arc_type(
                                        ait2->getSymbol(),
                                        state_type(ait1->getDest(), ait2->getDest())
                                    )
                                );
                            }
                            else if (ait2->getSymbol() == ANY) {
                                arcs.insert(
                                    arc_type(
                                        ait1->getSymbol(),
                                        state_type(ait1->getDest(), ait2->getDest())
                                    )
                                );
                            }
                        }
                    }
                }
            }
        }
        return arcs;
    }

  private:
    const FSA1 &m_fsa1;
    const FSA2 &m_fsa2;
    bool m_allowAny;
};

/*******************************************************************************
## Wrapper object: EpsRemover

template <typename FSA> class EpsRemover;

A wrapper object that provides a dynamically generated finite-state automaton
without epsilon transtions. The public members and types of this wrapper object
are common for all wrappers.

*******************************************************************************/
template <typename FSA>
class EpsRemover {
  public:
    typedef typename FSA::state_type state_type;
    typedef typename FSA::arc_type arc_type;
    typedef typename FSA::arc_iterator_type arc_iterator_type;

    EpsRemover(const FSA &fsa) : m_fsa(fsa) {}

    std::set<state_type> startFn() const {
        return m_fsa.getStartStates();
    }

    bool finalFn(state_type p) const {
        BOOST_FOREACH(state_type q, m_fsa.epsClosure(p))
            if (m_fsa.isEndState(q))
                return true;
        return false;
    }

    std::set<arc_type, ArcSorter> arcFn(State p) const {
        std::set<arc_type, ArcSorter> arcs;
        BOOST_FOREACH(state_type q, m_fsa.epsClosure(p)) {
            ArcRange<arc_iterator_type> arcRange = m_fsa.getArcs(q);
            for (arc_iterator_type ait = arcRange.first; ait != arcRange.second; ait++)
                if (ait->getSymbol() != EPS)
                    arcs.insert(*ait);
        }
        return arcs;
    }

  private:
    const FSA &m_fsa;
};

/*******************************************************************************
## Wrapper object: Determinizer

template <typename FSA> class Determinizer;

A wrapper object that provides are dynamically generated determinized finite-state
automaton. The public members and types of this wrapper object are common for all
wrappers.

*******************************************************************************/
template <typename FSA>
class Determinizer {
  public:
    typedef std::set<typename FSA::state_type> state_type;
    typedef Arc<typename FSA::arc_type::symbol_type, state_type> arc_type;

    Determinizer(const FSA &fsa) : m_fsa(fsa) {}

    std::set<state_type> startFn() const {
        std::set<state_type> start;
        start.insert(m_fsa.getStartStates());
        return start;
    }

    bool finalFn(state_type p) const {
        BOOST_FOREACH(typename FSA::state_type q, p)
            if (m_fsa.isEndState(q))
                return true;
        return false;
    }

    std::set<arc_type, ArcSorter> arcFn(state_type p) const {
        std::set<arc_type, ArcSorter> arcs;
        std::map<typename FSA::arc_type::symbol_type, arc_type> arcsMap;
        BOOST_FOREACH(typename FSA::state_type q, p) {
            ArcRange<typename FSA::arc_iterator_type> arcRange = m_fsa.getArcs(q);
            for (typename FSA::arc_iterator_type ait = arcRange.first;
                ait != arcRange.second; ait++) {

                arc_type& arc = arcsMap[ait->getSymbol()];
                arc.setSymbol(ait->getSymbol());
                const_cast<state_type&>(arc.getDest()).insert(ait->getDest());
            }
        }

        typedef typename std::map<typename FSA::arc_type::symbol_type, arc_type>
            ::value_type map_type;
        BOOST_FOREACH(map_type m, arcsMap)
            arcs.insert(m.second);

        return arcs;
    }

  private:
    const FSA &m_fsa;
};

/*******************************************************************************
## Reverse

template <typename FSA>
void reverse(FSA &fsa);

Reverses the given automaton.

*******************************************************************************/
template <typename FSA>
void reverse(FSA &fsa) {
    FSA temp;
    Reverser<FSA> rvr(fsa);
    traverse(rvr, temp);
    fsa.swap(temp);
}

/*******************************************************************************
## Reachable

template <typename FSA>
void reachable(FSA &fsa)

Keeps only reachable states.

*******************************************************************************/
template <typename FSA>
void reachable(FSA &fsa) {
    FSA temp;
    Copier<FSA> copier(fsa);
    traverse(copier, temp);
    fsa.swap(temp);
}

/*******************************************************************************
## Prune

template <typename FSA>
void prune(FSA &fsa)

Removes dead states, i.e. sink states that are not end states.

*******************************************************************************/
template <typename FSA>
void prune(FSA &fsa) {
    reverse(fsa);
    reachable(fsa);
    reverse(fsa);
}

/*******************************************************************************
## epsRemove

template <typename FSA>
void epsRemove(FSA &fsa)

Removes epsilon transitions from a non-deterministic automaton.

*******************************************************************************/
template <typename FSA>
void epsRemove(FSA &fsa) {
    FSA temp;
    EpsRemover<FSA> epsrm(fsa);
    traverse(epsrm, temp);
    fsa.swap(temp);
}

/*******************************************************************************
## Determinize

Determinizes a non-deterministic automaton. Requires removal of epsilon
transitions first if present.

*******************************************************************************/
template <typename FSA>
void determinize(FSA &fsa) {
    FSA temp;
    Determinizer<FSA> dtr(fsa);
    traverse(dtr, temp);
    fsa.swap(temp);
}

/*******************************************************************************
## Minimize

template <typename FSA>
void minimize(FSA &fsa)

Minimizes a non-deterministic automaton. Epsilon transtion removal and
determinization are automatically performed.

*******************************************************************************/
template <typename FSA>
void minimize(FSA &fsa) {
    epsRemove(fsa);
    reverse(fsa);
    determinize(fsa);
    reverse(fsa);
    determinize(fsa);
}

/*******************************************************************************
## Unify

template <typename FSA1, typename FSA2>
void unify(FSA1 &dst, FSA2 &src)

Creates the union of two finite state automata. The result is saved in the first
argument. This results in an automation with epsilon transtions.

*******************************************************************************/
template <typename FSA1, typename FSA2>
void unify(FSA1 &dst, FSA2 &src) {
    Copier<FSA2> copier(src);
    traverse(copier, dst);
}

/*******************************************************************************
## Intersect

template <typename FSA1, typename FSA2>
void intersect(FSA1 &dst, FSA2 &src)

Creates the intersection of two finite state automata. The result is saved in
the first argument.

*******************************************************************************/
template <typename FSA1, typename FSA2>
void intersect(FSA1 &dst, FSA2 &src, bool allowAny = false) {
    FSA1 temp;
    Intersector<FSA1, FSA2> intersector(dst, src, allowAny);
    traverse(intersector, temp);
    dst.swap(temp);
}

/*******************************************************************************
## Concatenate

template <typename FSA1, typename FSA2>
void concatenate(FSA1 &dst, FSA2 &src)

Creates the concatenation of two finite state automata. The result is saved in
the first argument. This results in an automaton with epsilon transtions.

*******************************************************************************/
template <typename FSA1, typename FSA2>
void concatenate(FSA1 &dst, FSA2 &src) {
    std::set<typename FSA1::state_type> first_start  = dst.getStartStates();
    std::set<typename FSA1::state_type> first_end    = dst.getEndStates();

    Copier<FSA2> copier(src);
    traverse(copier, dst);

    std::set<typename FSA1::state_type> second_start = dst.getStartStates();

    BOOST_FOREACH(typename FSA1::state_type p, first_end) {
        BOOST_FOREACH(typename FSA1::state_type q, second_start) {
            if (!first_start.count(q)) {
                dst.addArc(p, typename FSA1::arc_type(EPS, q));
                dst.unsetStartState(q);
            }
        }
        dst.unsetEndState(p);
    }
}

/*******************************************************************************
## Kleene Option

template <typename FSA>
void kleene_option(FSA &fsa)

Creates an optional finite state automaton. It accepts the language of the
argument automaton and the empty word. This results in an automaton with
epsilon transtions.

*******************************************************************************/
template <typename FSA>
void kleene_option(FSA &fsa) {
    const std::set<typename FSA::state_type> start  = fsa.getStartStates();
    const std::set<typename FSA::state_type> end    = fsa.getEndStates();

    typename FSA::state_type ns = fsa.addState();
    fsa.setStartState(ns);

    typename FSA::state_type ne = fsa.addState();
    fsa.setEndState(ne);

    BOOST_FOREACH(typename FSA::state_type p, start) {
        fsa.unsetStartState(p);
        fsa.addArc(ns, typename FSA::arc_type(EPS, p));
    }

    BOOST_FOREACH(typename FSA::state_type p, end) {
        fsa.unsetEndState(p);
        fsa.addArc(p, typename FSA::arc_type(EPS, ne));
    }
}

/*******************************************************************************
## Kleene plus

template <typename FSA>
void kleene_plus(FSA &fsa)

Creates the non-empty Kleene closure of the passed automaton. The result will
accept the language of the input automaton repeated one to infinite times. This
results in an automaton with epsilon transtions.

*******************************************************************************/
template <typename FSA>
void kleene_plus(FSA &fsa) {
    const std::set<typename FSA::state_type> &start  = fsa.getStartStates();
    const std::set<typename FSA::state_type> &end    = fsa.getEndStates();

    BOOST_FOREACH(typename FSA::state_type p, end)
        BOOST_FOREACH(typename FSA::state_type q, start)
            fsa.addArc(p, typename FSA::arc_type(EPS, q));
}

/*******************************************************************************
## Kleene star

template <typename FSA>
void kleene_star(FSA &fsa)

Similar as above, but accepts also the empty string. This results in an
automaton with epsilon transtions.

*******************************************************************************/
template <typename FSA>
void kleene_star(FSA &fsa) {
    kleene_option(fsa);
    kleene_plus(fsa);
}

/*******************************************************************************
## Minimize incremental

template <typename DFSA, typename Iterator>
void minimize_incremental(DFSA &dst, Iterator current, Iterator end)

Create inrementally minimal DFSA from sorted collection of words. Implementation
of Jan Daciuk's algorithm for sorted data

*******************************************************************************/

template <typename DFSA> class Register;

template <typename DFSA, typename Iterator>
void minimize_incremental(DFSA &dst, Iterator current, Iterator end) {
    typedef typename DFSA::state_type State;
    typedef typename DFSA::arc_type Arc;
    typedef typename DFSA::arc_iterator_type ArcIt;

    typedef typename boost::optional<State> OptState;

    INFO("Building minimial finite state automaton");

    DFSA fsa;
    State q0 = fsa.addState(1);

    Register<DFSA> state_register(fsa);

    size_t count = 0;
    while (current != end) {
        typename Iterator::value_type::const_iterator
            curr_symbol_it = current->begin();

        OptState curr_state(q0), next_state;
        while (curr_symbol_it != current->end() &&
              (next_state = fsa.delta(curr_state.get(), *curr_symbol_it))) {
            //std::cerr << "curr symbol: " << (int)*curr_symbol_it << std::endl;
            curr_state = next_state;
            curr_symbol_it++;
        }

        State last_state = curr_state.get();
        ArcRange<ArcIt> range = fsa.getArcs(last_state);
        if (range.first != range.second)
            replace_or_register(fsa, state_register, last_state);

        while (curr_symbol_it != current->end()) {
            State dst_state = fsa.addState();
            fsa.addArc(last_state, Arc(*curr_symbol_it, dst_state));
            last_state = dst_state;
            curr_symbol_it++;
        }
        fsa.setEndState(last_state);
        current++;
        count++;
        if (count % 1000000 == 0)
            INFO("[" << count << "]");
    }
    replace_or_register(fsa, state_register, q0);
    dst.swap(fsa);

    INFO("Done");
}

template <typename DFSA, typename Register>
void replace_or_register(DFSA &fsa,
                         Register &state_register,
                         typename DFSA::state_type state) {

    typedef typename DFSA::state_type State;
    typedef typename DFSA::arc_type Arc;
    typedef typename DFSA::arc_iterator_type ArcIt;

    //std::cerr << "replace or register: " << state << std::endl;

    ArcIt last_arc = fsa.getArcs(state).second-1;
    State last_child = last_arc->getDest();
    ArcRange<ArcIt> range = fsa.getArcs(last_child);
    if (range.first != range.second)
        replace_or_register(fsa, state_register, last_child);

    //std::cerr << "Checking: " << last_child << std::endl;
    //std::cerr << "Size: " << state_register.size() << std::endl;
    typename Register::iterator q = state_register.find(last_child);
    if (q != state_register.end()) {
        //std::cerr << "Found: " << *q << std::endl;
        const_cast<Arc&>(*last_arc).setDest(*q);
        fsa.deleteLastState();
    }
    else
        state_register.insert(last_child);
}

/*******************************************************************************
## Minimize incremental

template <typename DFSA, typename Iterator>
void minimize_incremental_unsorted(DFSA &dst, Iterator current, Iterator end)

Create inrementally minimal DFSA from unsorted collection of words. Implementation
of Jan Daciuk's algorithm for unsorted data

*******************************************************************************/

/* @TODO: finish algorithm
template <typename DFSA, typename Iterator>
void minimize_incremental_unsorted(DFSA &dst, Iterator current, Iterator end) {
    typedef typename DFSA::state_type State;
    typedef typename DFSA::symbol_type Symbol;
    typedef typename DFSA::arc_type Arc;
    typedef typename DFSA::arc_iterator_type ArcIt;

    typedef typename boost::optional<State> OptState;

    INFO("Building minimial finite state automaton");

    boost::unordered_map<State, size_t> confluenceStates;

    DFSA fsa;
    State q0 = fsa.addState(1);

    Register<DFSA> state_register(fsa);

    size_t count = 0;
    while (current != end) {
        typename Iterator::value_type::const_iterator
            curr_symbol_it = current->begin();

        OptState curr_state(q0), next_state, first_state;
        while (curr_symbol_it != current->end() &&
              (next_state = fsa.delta(curr_state.get(), *curr_symbol_it))) {
            //std::cerr << "curr symbol: " << (int)*curr_symbol_it << std::endl;
            curr_state = next_state;
            if (confluenceStates.count(curr_state.get())
               && confluenceStates[curr_state.get()] > 1)
                first_state = curr_state;

            curr_symbol_it++;
        }

        typename Iterator::value_type::const_iterator
            prefix_end = curr_symbol_it;

        size_t prefixLength = std::distance(current->begin(), prefix_end);

        if (prefix_end == current->end() && fsa.isEndState(curr_state.get()))
            continue;

        State last_state;
        if (!first_state) {
            last_state = curr_state.get();
        }
        else {
            last_state = fsa.addState();
            ArcRange<ArcIt> range = fsa.getArcs(curr_state.get());
            for (ArcIt arcIt = range.first; arcIt != range.second; arcIt++)
                fsa.addArc(last_state, Arc(arcIt->getSymbol(), arcIt->getDest()));
        }

        // add_suffx(...)
        while (curr_symbol_it != current->end()) {
            State dst_state = fsa.addState();
            fsa.addArc(last_state, Arc(*curr_symbol_it, dst_state));
            last_state = dst_state;
            curr_symbol_it++;
        }
        fsa.setEndState(last_state);

        State currentState;
        size_t currentIndex;
        if (first_state) {
        // ...
            for (size_t i = prefixLength - 1; i >= currentIndex; i--) {
                currentState = fsa.addState();
                ArcRange<ArcIt> range = fsa.getArcs(last_state);
                for (ArcIt arcIt = range.first; arcIt != range.second; arcIt++)
                    fsa.addArc(last_state, Arc(arcIt->getSymbol(), arcIt->getDest()));


                Symbol currentSymbol = *(current->begin() + i + 1);
                replace_or_register(fsa, state_register, currentState, currentSymbol);
                last_state = currentState;
            }
        }
        else {
            currentIndex = prefixLength;
        }

        bool changed = true;
        while (changed) {
            currentIndex = currentIndex - 1;

            typename Iterator::value_type::const_iterator
            curr_symbol_it = current->begin();

            typename Iterator::value_type::const_iterator
            stop = current->begin() + currentIndex;

            OptState curr_state(q0), next_state;
            while (curr_symbol_it != stop &&
                  (next_state = fsa.delta(curr_state.get(), *curr_symbol_it))) {
                curr_state = next_state;
                curr_symbol_it++;
            }
            currentState = curr_state.get();

            State old_state = last_state;

            if (currentIndex > 0)
                state_register.erase(last_state);

            Symbol currentSymbol = *(current->begin() + currentIndex + 1);
            replace_or_register(fsa, state_register, currentState, currentSymbol);

            last_state = fsa.delta(currentState, currentSymbol);
            changed = last_state != old_state;
            last_state = currentState;
        }
        if (!changed && currentIndex > 0)
            state_register.insert(currentState);

        count++;
        if (count % 1000000 == 0)
            INFO("[" << count << "]");

        current++;
    }

    dst.swap(fsa);
    INFO("Done");
}

template <typename DFSA, typename Register>
void replace_or_register(DFSA &fsa, Register &state_register, typename DFSA::state_type state,
    typename DFSA::symbol_type symbol,
    boost::unordered_map<typename DFSA::state_type, size_t>& confluenceStates) {

    typedef typename DFSA::state_type State;
    typedef typename DFSA::symbol_type Symbol;
    typedef typename DFSA::arc_type Arc;
    typedef typename DFSA::arc_iterator_type ArcIt;

    ArcIt last_arc = fsa.getArcs(state).second-1;
    State child = fsa.delta(state, symbol).get();

    typename Register::iterator q = state_register.find(child);
    if (q != state_register.end()) {
        const_cast<Arc&>(*last_arc).setDest(*q);
        confluenceStates[*q]++;
        confluenceStates.erase(child);
        //fsa.deleteState(child);
    }
    else
        state_register.insert(child);
}
*/

/***********************************************************************/

template <typename DFSA>
class StateHash : std::unary_function<typename DFSA::state_type, size_t>
{
  public:
    StateHash(DFSA &fsa)
      : fsa_(fsa)
    { }

    size_t operator()(typename DFSA::state_type const& s) const {
        std::size_t seed = 0;

        boost::hash_combine(seed, fsa_.isEndState(s));
        ArcRange<typename DFSA::arc_iterator_type> range = fsa_.getArcs(s);
        while (range.first != range.second) {
            boost::hash_combine(seed, range.first->getSymbol());
            boost::hash_combine(seed, range.first->getDest());
            range.first++;
        }

        return seed;
    }

  private:
    DFSA& fsa_;

};

template <typename DFSA>
class StateEquiv : std::binary_function<typename DFSA::state_type,
                                         typename DFSA::state_type, bool>
{
  public:

    StateEquiv(DFSA &fsa)
      : fsa_(fsa)
    { }

    bool operator()(typename DFSA::state_type const& s1,
               typename DFSA::state_type const& s2) const {

        //std::cerr << "Comparing " << s1 << " " << s2 << std::endl;
        bool es1 = fsa_.isEndState(s1);
        bool es2 = fsa_.isEndState(s2);
        //std::cerr << "End states: " << es1 << " " << es2 << std::endl;
        if (es1 != es2) {
            //std::cerr << "Return " << false << std::endl;
            return false;
        }

        ArcRange<typename DFSA::arc_iterator_type>
        range1 = fsa_.getArcs(s1);

        ArcRange<typename DFSA::arc_iterator_type>
        range2 = fsa_.getArcs(s2);

        ArcSorter arc_cmp;
        bool result1 = std::lexicographical_compare(range1.first, range1.second,
            range2.first, range2.second, arc_cmp);
        bool result2 = std::lexicographical_compare(range2.first, range2.second,
            range1.first, range1.second, arc_cmp);

        //std::cerr << "Less: " << result1 << " " << result2 << std::endl;
        bool result = (!result1 && !result2);
        //std::cerr << "Return " << result << std::endl;
        return result;
    }

  private:
    DFSA& fsa_;
};

template <typename DFSA>
class Register : public boost::unordered_set<typename DFSA::state_type,
        StateHash<DFSA>, StateEquiv<DFSA> >
{
  public:
    Register(DFSA &fsa) :
      boost::unordered_set<typename DFSA::state_type,
      StateHash<DFSA>, StateEquiv<DFSA> >(100, StateHash<DFSA>(fsa),
      StateEquiv<DFSA>(fsa)),
      fsa_(fsa) { }

  private:
    DFSA &fsa_;
};


template <typename FSA, typename Iterator, typename Word>
void language_rec(FSA &fsa, typename FSA::state_type state, Word word,
                  Iterator it, size_t maxDepth) {
    if (word.size() > maxDepth)
        return;

    if (fsa.isEndState(state))
        *it = word;

    ArcRange<typename FSA::arc_iterator_type> r = fsa.getArcs(state);
    while (r.first != r.second) {
        Word temp_word = word;
        temp_word.push_back(r.first->getSymbol());
        language_rec(fsa, r.first->getDest(), temp_word, it, maxDepth);
        r.first++;
    }
}

template <typename FSA, typename Iterator, typename Word>
void language(FSA &fsa, Iterator it, size_t maxDepth = 10000) {
    Word word;
    BOOST_FOREACH(typename FSA::state_type state, fsa.getStartStates())
        language_rec(fsa, state, word, it, maxDepth);
}

}


#endif
