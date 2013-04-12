#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>

#include "fsa_multi_store.hpp"

class Line : public std::string {
  public:
    friend std::istream &operator>>(std::istream &is, Line &line) {
        std::getline(is, line);
        return is;
    }
};

int main(int argc, char** argv) {
    psi::fsa::FSAMultiStore::Builder builder("\t");
    builder.createFromSeparatedSortedLines(std::istream_iterator<Line>(std::cin),
                                           std::istream_iterator<Line>());
    psi::fsa::FSAMultiStore *store = builder.build();
    store->save(argv[1]);
}
