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

class Dictionary
{
  public:
    Dictionary();

    LemmaMap get(const std::string&);

    void readDictionary(const std::string&);

    void load(std::string);
    void load(std::ifstream&);

    void save(std::string);
    void save(std::ofstream&);

    size_t isEmpty() { return !store_; }

  private:
    boost::shared_ptr<fsa::FSAMultiStore> store_;
};


#endif
