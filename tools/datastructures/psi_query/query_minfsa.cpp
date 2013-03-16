#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>

#include "fsa_multi_store.hpp"

int main(int argc, char** argv) {
    psi::fsa::FSAMultiStore store;
    store.load(argv[1]);
    
    std::string line;
    while(std::getline(std::cin, line)) {
        BOOST_FOREACH(std::string word, store.get(line))
            std::cout << word << std::endl;
    }
}

