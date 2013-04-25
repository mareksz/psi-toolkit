#ifndef EDGE_PRINT_DATA_HDR
#define EDGE_PRINT_DATA_HDR


#include <string>

#include <boost/optional.hpp>

#include "edge_data.hpp"
#include "lattice.hpp"


struct EdgePrintData {
    boost::optional<Lattice::EdgeDescriptor> source;
    boost::optional<Lattice::EdgeDescriptor> parent;
    std::string text;

    EdgePrintData() :
        text("")
    { }

    EdgePrintData(
        EdgeData & data,
        std::string a_text
    ) :
        source(data.source),
        parent(data.parent),
        text(a_text)
    { }

    bool operator<(const EdgePrintData & other) const {
        return text < other.text;
    }
};


#endif
