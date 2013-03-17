#ifndef DICTIONARY_HDR
#define DICTIONARY_HDR

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <iostream>

#include <boost/foreach.hpp>

#include "fsa_multi_store.hpp"
#include "utility_functions.hpp"

namespace psi {
    
    class Dictionary
    {
      public:
        Dictionary(bool hasPos = false, bool hasMorpho = false);
        ~Dictionary();
        
        LemmaMap get(const std::string&);
        
        void readDictionary(const std::string&);
        
        void load(std::string);
        void load(std::ifstream&);
        
        void save(std::string);
        void save(std::ofstream&);
        
        bool hasPos() { return hasPos_; }
        bool hasMorpho() { return hasMorpho_; }
        
        size_t isEmpty() { return !store_; }
        
      private:
        bool hasPos_;
        bool hasMorpho_;
        
        fsa::FSAMultiStore* store_;
    };

}
#endif
