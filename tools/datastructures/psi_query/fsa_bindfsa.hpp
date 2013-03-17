#ifndef BinDFSA_HDR
#define BinDFSA_HDR

#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>
#include <fstream>

#include <boost/foreach.hpp>
#include <boost/optional.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#include "fsa_types.hpp"
#include "fsa_mmap_allocator.hpp"

namespace psi {
  namespace fsa {

    template <typename ArcT = Arc<Symbol, State>, typename PosT = size_t,
    template <typename> class Allocator = std::allocator>
    class BinDFSA {
      protected:
        typedef std::vector<PosT, Allocator<PosT> > States;
        typedef std::vector<ArcT, Allocator<ArcT> > Arcs;

      public:
        typedef size_t state_type;
        typedef ArcT arc_type;
        typedef typename ArcT::symbol_type symbol_type;
        typedef typename Arcs::const_iterator arc_iterator_type;

      protected:
        States m_states;
        Arcs m_arcs;

        PosT setLastBit(PosT) const;
        PosT unsetLastBit(PosT) const;

        arc_iterator_type find(size_t, symbol_type) const;

      public:
        BinDFSA();
        BinDFSA(std::string);

        boost::optional<state_type> delta(state_type p, symbol_type a) const {
           if(m_states.size() < p)
               return boost::optional<state_type>();
   
           arc_iterator_type arc = find(p, a);
           if (arc == m_arcs.end())
               return boost::optional<state_type>();
           else
               return boost::optional<state_type>(arc->getDest());
       }

        template <typename InputIterator>
        bool in(InputIterator, InputIterator);

        const std::set<size_t> getStartStates() const;
        bool isStartState(size_t) const;

        bool isEndState(size_t) const;
        const std::set<size_t> getEndStates() const;

        const ArcRange<arc_iterator_type> getArcs(size_t) const;

        void setStartState(size_t);
        void unsetStartState(size_t);

        void setEndState(size_t);
        void unsetEndState(size_t);

        size_t addState(bool = false);
        void addArc(size_t, arc_type);
        
        void deleteLastState();

        std::set<size_t> epsClosure(size_t p) const;

        size_t size() const;
        void swap(BinDFSA &);
        void print();

        template <class IStream>
        void load(IStream& istream);
        void load(std::string);

        template <class OStream>
        void save(OStream& ostream);
        void save(std::string);

    };

    template <typename ArcT, typename PosT, template <typename> class Allocator>
    BinDFSA<ArcT, PosT, Allocator>::BinDFSA() { }
    
    template <typename ArcT, typename PosT, template <typename> class Allocator>
    BinDFSA<ArcT, PosT, Allocator>::BinDFSA(std::string filename) {
        load(filename);
    }
    
    template <typename ArcT, typename PosT, template <typename> class Allocator>
    const std::set<size_t> BinDFSA<ArcT, PosT, Allocator>::getStartStates() const {
        std::set<size_t> startStates;
        startStates.insert(0);
        return startStates;
    };

    template <typename ArcT, typename PosT, template <typename> class Allocator>
    bool BinDFSA<ArcT, PosT, Allocator>::isStartState(size_t p) const {
        return p == 0;
    }

    template <typename ArcT, typename PosT, template <typename> class Allocator>
    void BinDFSA<ArcT, PosT, Allocator>::setStartState(size_t p) {}

    template <typename ArcT, typename PosT, template <typename> class Allocator>
    void BinDFSA<ArcT, PosT, Allocator>::unsetStartState(size_t p) {}

    template <typename ArcT, typename PosT, template <typename> class Allocator>
    void BinDFSA<ArcT, PosT, Allocator>::setEndState(size_t p) {
        m_states[p] = setLastBit(m_states[p]);
    }

    template <typename ArcT, typename PosT, template <typename> class Allocator>
    void BinDFSA<ArcT, PosT, Allocator>::unsetEndState(size_t p) {
        m_states[p] = unsetLastBit(m_states[p]);
    }

    template <typename ArcT, typename PosT, template <typename> class Allocator>
    size_t BinDFSA<ArcT, PosT, Allocator>::addState(bool start) {
        size_t p = m_states.size();
        m_states.resize(p+1);
        m_states[p] = m_arcs.size();
        return p;
    }

    template <typename ArcT, typename PosT, template <typename> class Allocator>
    void BinDFSA<ArcT, PosT, Allocator>::addArc(size_t p, ArcT a) {
        //std::cerr << "Adding " << p << " " << a.getDest() << " " << (int)a.getSymbol() << std::endl;
      
        ArcSorter cmp;
        if (p < m_states.size()) {
            PosT pos = unsetLastBit(m_states[p]);
            PosT end = m_arcs.size();

            if (p < m_states.size()-1)
                end = unsetLastBit(m_states[p+1]);

            while (pos < end && cmp(m_arcs[pos], a))
                pos++;

            // Bugfix for the allocator, flawed insertion on reallocation
            if (m_arcs.capacity() == m_arcs.size())
                m_arcs.reserve(m_arcs.size()*2);
            m_arcs.insert(m_arcs.begin() + pos, a);

            for (size_t q = p + 1; q < m_states.size(); q++)
                if (isEndState(q))
                    m_states[q] = setLastBit(unsetLastBit(m_states[q]) + 1);
                else
                    m_states[q]++;
        }
    }
    
    template <typename ArcT, typename PosT, template <typename> class Allocator>
    void BinDFSA<ArcT, PosT, Allocator>::deleteLastState() {
      size_t last_size = unsetLastBit(m_states.back());
      
      //std::cerr << m_states.size() << " " << last_size << " " << m_arcs.size() << std::endl;
      
      m_states.pop_back();
      m_arcs.resize(last_size);
    }

    template <typename ArcT, typename PosT, template <typename> class Allocator>
    inline typename BinDFSA<ArcT, PosT, Allocator>::arc_iterator_type
    BinDFSA<ArcT, PosT, Allocator>::find(size_t p, typename ArcT::symbol_type a) const {
        ArcRange<arc_iterator_type> r = getArcs(p);

        ArcT test(a, 0);
        arc_iterator_type result = std::lower_bound(r.first, r.second, test, ArcSorter());
        
        //std::cerr << "Arc find: " << (int)a << " " << std::distance(r.first, result) << std::endl;
        //std::cerr << "Check: " << (result != r.second && a == result->getSymbol()) << std::endl;
        return (result != r.second && a == result->getSymbol()) ? result : m_arcs.end();
    }

    template <typename ArcT, typename PosT, template <typename> class Allocator>
    template <typename InputIterator>
    bool BinDFSA<ArcT, PosT, Allocator>::in(InputIterator it, InputIterator end) {
        state_type current_state = 0;
        while (it != end) {
            boost::optional<state_type> next_state = delta(current_state, *it);
            if (next_state)
                current_state = next_state.get();
            else
                return false;
            it++;
        }
        return isEndState(current_state);
    }

    template <typename ArcT, typename PosT, template <typename> class Allocator>
    PosT BinDFSA<ArcT, PosT, Allocator>::setLastBit(PosT pos) const {
        return pos | (PosT(1) << (sizeof(PosT)*8-1));
    }

    template <typename ArcT, typename PosT, template <typename> class Allocator>
    PosT BinDFSA<ArcT, PosT, Allocator>::unsetLastBit(PosT pos) const {
        return pos & ~(PosT(1) << (sizeof(PosT)*8-1));
    }


    template <typename ArcT, typename PosT, template <typename> class Allocator>
    bool BinDFSA<ArcT, PosT, Allocator>::isEndState(size_t state) const {
        return (m_states[state] & (PosT(1) << (sizeof(PosT)*8-1)));
    }

    template <typename ArcT, typename PosT, template <typename> class Allocator>
    const std::set<size_t> BinDFSA<ArcT, PosT, Allocator>::getEndStates() const {
        std::set<size_t> end_states;

        for (size_t p = 0; p < m_states.size(); p++)
            if (isEndState(p))
                end_states.insert(p);

        return end_states;
    }

    template <typename ArcT, typename PosT, template <typename> class Allocator>
    const ArcRange<typename BinDFSA<ArcT, PosT, Allocator>::arc_iterator_type>
    BinDFSA<ArcT, PosT, Allocator>::getArcs(size_t p) const {
        if (p < m_states.size()-1) {
            PosT pos1 = unsetLastBit(m_states[p]);
            PosT pos2 = unsetLastBit(m_states[p+1]);

            arc_iterator_type ait1 = m_arcs.begin();
            arc_iterator_type ait2 = m_arcs.begin();

            ait1 += pos1;
            ait2 += pos2;

            return ArcRange<arc_iterator_type>(
                ait1,
                ait2
            );
        }
        else if (p == m_states.size()-1) {
            PosT pos = unsetLastBit(m_states[p]);
            arc_iterator_type ait = m_arcs.begin();
            ait += pos;

            return ArcRange<arc_iterator_type>(
                ait,
                m_arcs.end()
            );
        }
        else {
            return ArcRange<arc_iterator_type>(
                m_arcs.end(),
                m_arcs.end()
            );
        }
    }

    template <typename ArcT, typename PosT, template <typename> class Allocator>
    size_t BinDFSA<ArcT, PosT, Allocator>::size() const {
        return m_states.size();
    }

    template <typename ArcT, typename PosT, template <typename> class Allocator>
    void BinDFSA<ArcT, PosT, Allocator>::swap(BinDFSA<ArcT, PosT, Allocator> &fsa) {
        m_states.swap(fsa.m_states);
        m_arcs.swap(fsa.m_arcs);
    }

    template <typename ArcT, typename PosT, template <typename> class Allocator>
    void BinDFSA<ArcT, PosT, Allocator>::print() {
        for (size_t p = 0; p < m_states.size(); p++) {
            ArcRange<arc_iterator_type> r = getArcs(p);
            for (arc_iterator_type it = r.first; it != r.second; it++) {
                std::cout << p << "\t" << it->getDest() << "\t" << (long)it->getSymbol()
                    << std::endl;
            }
            if (isEndState(p)) {
                std::cout << p << std::endl;
            }
        }
    }

    template <typename ArcT, typename PosT, template <typename> class Allocator>
    std::set<size_t> BinDFSA<ArcT, PosT, Allocator>::epsClosure(size_t p) const {
        std::set<size_t> closure;
        std::vector<size_t> queue;

        closure.insert(p);
        queue.push_back(p);
        while (!queue.empty()) {
            size_t s = queue.back();
            queue.pop_back();

            ArcRange<arc_iterator_type> arcs = getArcs(s);
            for (arc_iterator_type ait = arcs.first; ait != arcs.second; ait++) {
                if (ait->getSymbol() == EPS && !closure.count(ait->getDest())) {
                    closure.insert(ait->getDest());
                    queue.push_back(ait->getDest());
                }
            }
        }

        return closure;
    }

    template <typename ArcT, typename PosT, template <typename> class Allocator>
    void BinDFSA<ArcT, PosT, Allocator>::load(std::string filename) {
        std::ifstream in(filename.c_str(), std::ios_base::in|std::ios_base::binary);
        load(in);
        in.close();
    }
    
    template <typename ArcT, typename PosT, template <typename> class Allocator>
    template <class IStream>
    void BinDFSA<ArcT, PosT, Allocator>::load(IStream& in) {
      std::streampos end;
      in.read((char*) &end, sizeof(std::streampos));
      {
          boost::iostreams::filtering_istream istream;
          istream.push(boost::iostreams::gzip_decompressor());
          istream.push(in);    
          
          size_t statesNum;
          istream.read((char*)&statesNum, sizeof(size_t));
          m_states.resize(statesNum);
          istream.read((char*)&m_states[0], statesNum * sizeof(PosT));
    
          size_t arcsNum;
          istream.read((char*)&arcsNum, sizeof(size_t));
          m_arcs.resize(arcsNum);
          istream.read((char*)&m_arcs[0], arcsNum * sizeof(ArcT));
      }
      in.seekg(end);
    }

    template <typename ArcT, typename PosT, template <typename> class Allocator>
    void BinDFSA<ArcT, PosT, Allocator>::save(std::string filename) {
        std::ofstream out(filename.c_str(), std::ios_base::out|std::ios_base::binary);
        save(out);
        out.close();
    }
    
    template <typename ArcT, typename PosT, template <typename> class Allocator>
    template <class OStream>
    void BinDFSA<ArcT, PosT, Allocator>::save(OStream& out) {
      
      std::streampos start = out.tellp();
      out.write((char*) &start, sizeof(std::streampos)); //make some space;
      {
          boost::iostreams::filtering_ostream ostream;
          ostream.push(boost::iostreams::gzip_compressor());
          ostream.push(out);
          
          size_t statesNum = m_states.size();
          ostream.write((char*)&statesNum, sizeof(size_t));
          ostream.write((char*)&m_states[0], statesNum * sizeof(PosT));
    
          size_t arcsNum = m_arcs.size();
          ostream.write((char*)&arcsNum, sizeof(size_t));
          ostream.write((char*)&m_arcs[0], arcsNum * sizeof(ArcT));
      }
      // Some magic to remember the exact gzip stream end:
      std::streampos end = out.tellp();
      out.seekp(start); //jump to start;
      out.write((char*) &end, sizeof(std::streampos)); //save end position;
      out.seekp(end); //jump to end;
    }

    typedef BinDFSA<Arc<Symbol, unsigned>, unsigned, std::allocator> MemBinDFSA;
    typedef BinDFSA<Arc<Symbol, unsigned>, unsigned, MmapAllocator>  MapBinDFSA;
    
    typedef BinDFSA<Arc<unsigned char, unsigned>, unsigned> StringBinFSA;

  }
}

#endif
