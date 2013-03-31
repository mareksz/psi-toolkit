#ifndef FSA_NDFSA_HDR
#define FSA_NDFSA_HDR

#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>
#include <boost/foreach.hpp>

#include "fsa_types.hpp"
#include "fsa_mmap_allocator.hpp"

namespace fsa {

template <typename ArcT = Arc<Symbol, State> >
class NDFSA {
  protected:
    typedef std::vector<ArcT> Arcs;
    typedef std::vector<Arcs*> States;

  public:
    typedef typename ArcT::state_type state_type;
    typedef ArcT arc_type;
    typedef typename ArcT::symbol_type symbol_type;
    typedef typename Arcs::iterator arc_iterator_type;

  protected:
    States m_states;

    std::set<state_type> m_startStates;
    std::set<state_type> m_endStates;

    arc_iterator_type find(state_type, symbol_type) const;

  public:
        
/*******************************************************************************
## Constructor

  NDFSA();

Basic constructor. Creates an empty non-deterministic finite-state automaton.

*******************************************************************************/
    NDFSA();

/*******************************************************************************
## Constructor

  NDFSA(symbol_type);

Another contrustor. Creates an non-deterministic finite-state automaton consisting
of two states and one transition labelled with the argument symbol.

*******************************************************************************/
    NDFSA(symbol_type);

/*******************************************************************************
## Constructor

  NDFSA();

Another constructor. Creates a sequential non-deterministic finite-state automaton
from the iterators given as arguments.

Example:

  std::string input = "this is an example";
  NDFSA example(input.begin(), input.end());
  std::cout << example.in(input) << std::cout; // prints "1"

*******************************************************************************/
    template <typename InputIterator>
    NDFSA(InputIterator, InputIterator);
        
/*******************************************************************************
## Constructor

  NDFSA();

Another constructor. Creates a sequential non-deterministic finite-state automaton
from the string given as argument.

Example:

  std::string input = "this is an example";
  NDFSA example(input);
  std::cout << example.in(input) << std::cout; // prints "1"

*******************************************************************************/
    NDFSA(std::string);

/*******************************************************************************
## Destructor

  ~NDFSA();
*******************************************************************************/
    ~NDFSA();


/*******************************************************************************
## Delta transtion function over sets of states

    std::set<state_type> delta(std::set<state_type>, symbol_type) const;
*******************************************************************************/
    std::set<state_type> delta(std::set<state_type>, symbol_type) const;

/*******************************************************************************
## Delta prime transition function over single states

    std::set<state_type> deltaPrime(state_type, symbol_type) const;

*******************************************************************************/
    std::set<state_type> deltaPrime(state_type, symbol_type) const;

/*******************************************************************************
## Epsilon closure for given state

    std::set<state_type> epsClosure(state_type) const;

*******************************************************************************/
    std::set<state_type> epsClosure(state_type) const;

/*******************************************************************************
## Inclusion function

    template <typename InputIterator> bool in(InputIterator, InputIterator);

Checks whether given sequence of labels belongs to the language of the automaton.
    
*******************************************************************************/
    template <typename InputIterator>
    bool in(InputIterator, InputIterator);

/*******************************************************************************
## Add single state to automaton.

    state_type addState(bool = false);

Creates a single new state. If argument is set to true, an end state is created.
The state identifier is returned.
    
*******************************************************************************/
    state_type addState(bool = false);

/*******************************************************************************
## Delete highest numbered state from automaton.

    void deleteLastState();

*******************************************************************************/
    void deleteLastState();
    
/*******************************************************************************
## Get set of start states

    const std::set<state_type>& getStartStates() const;
*******************************************************************************/
    const std::set<state_type>& getStartStates() const;

/*******************************************************************************
## Check for start state

    bool isStartState(state_type) const;

*******************************************************************************/
    bool isStartState(state_type) const;

/*******************************************************************************
## Set state as start state

    void setStartState(state_type);

*******************************************************************************/
    void setStartState(state_type);

/*******************************************************************************
## Remove state from set of start states

    void unsetStartState(state_type);

*******************************************************************************/
    void unsetStartState(state_type);

/*******************************************************************************
## Get set of end states

    const std::set<state_type>& getEndStates() const;

*******************************************************************************/
    const std::set<state_type>& getEndStates() const;

/*******************************************************************************
## Check if state is end state

    bool isEndState(state_type) const;

*******************************************************************************/
    bool isEndState(state_type) const;

/*******************************************************************************
## Set state as end state

    void setEndState(state_type);

*******************************************************************************/
    void setEndState(state_type);

/*******************************************************************************
## Remove state from set of end states

    void unsetEndState(state_type);

*******************************************************************************/
    void unsetEndState(state_type);

/*******************************************************************************
## Add arc

    void addArc(state_type, arc_type);

Add arc beginning in given state.
    
*******************************************************************************/
    void addArc(state_type, arc_type);

/*******************************************************************************
## Get range of out-arcs

    const ArcRange<arc_iterator_type> getArcs(state_type) const;

*******************************************************************************/
    const ArcRange<arc_iterator_type> getArcs(state_type) const;

/*******************************************************************************
## Return number of states

    size_t size() const;

*******************************************************************************/
        size_t size() const;

/*******************************************************************************
## Replace current finite-state automaton with the argument. All references,
pointers, iterators to states and transitions will be invalidated.

    size_t swap(NDFSA &);

*******************************************************************************/
    void swap(NDFSA &);

/*******************************************************************************
## Print out ascii representation of underlying automaton
    void print();

*******************************************************************************/
    void print();

    NDFSA<arc_type>& operator=(const NDFSA<arc_type> &);

};

typedef NDFSA<Arc<unsigned char, unsigned> > StringFSA;

}

#include "fsa_ndfsa_impl.hpp"

#endif
