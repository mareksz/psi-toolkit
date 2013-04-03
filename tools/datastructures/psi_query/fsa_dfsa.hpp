#ifndef FSA_DFSA_HDR
#define FSA_DFSA_HDR

#include <boost/optional.hpp>

#include "fsa_ndfsa.hpp"


namespace fsa {

template <typename ArcT = Arc<Symbol, State> >
class DFSA : public NDFSA<ArcT> {
  protected:
    typedef typename NDFSA<ArcT>::Arcs Arcs;
    typedef typename NDFSA<ArcT>::States States;

  public:
    typedef typename NDFSA<ArcT>::state_type state_type;
    typedef typename NDFSA<ArcT>::arc_type arc_type;
    typedef typename NDFSA<ArcT>::symbol_type symbol_type;
    typedef typename NDFSA<ArcT>::arc_iterator_type arc_iterator_type;

/*******************************************************************************
## Constructor

DFSA();

Basic constructor. Creates a deterministic finite-state automaton and an
underlying non-deterministic finite-state automaton.

*******************************************************************************/
    DFSA() : NDFSA<ArcT>::NDFSA() {};

    DFSA<ArcT>& operator=(const NDFSA<ArcT> &ndfsa) {
        NDFSA<ArcT>::operator=(ndfsa);
        return *this;
    }

/*******************************************************************************
## Deterministic inclusion function

template <typename InputIterator> bool in(InputIterator, InputIterator);

Checks whether given sequence of labels belongs to the language of the automaton.

*******************************************************************************/
    template <typename InputIterator>
    bool in(InputIterator it, InputIterator end) {
        state_type current_state = 0;
        while (it != end) {
            boost::optional<state_type> next_state = delta(current_state, *it);
            if (next_state)
                current_state = next_state.get();
            else
                return false;
            it++;
        }
        return this->isEndState(current_state);
    }

/*******************************************************************************
## Deterministic delta transtion function

state_type delta(state_type p, symbol_type a) const;

*******************************************************************************/
    boost::optional<state_type> delta(state_type p, symbol_type a) const {
        if (NDFSA<ArcT>::m_states.size() < (size_t)p)
            return boost::optional<state_type>();

        //std::cerr << "Looking for: " << p << " " << (int)a << std::endl;
        arc_iterator_type arc = this->find(p, a);
        if (arc == NDFSA<ArcT>::m_states[p]->end())
            return boost::optional<state_type>();
        else
            return boost::optional<state_type>(arc->getDest());
    }
};

}


#endif
