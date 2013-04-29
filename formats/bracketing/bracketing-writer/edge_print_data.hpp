#ifndef EDGE_PRINT_DATA_HDR
#define EDGE_PRINT_DATA_HDR


#include <string>

#include <boost/optional.hpp>

#include "edge_data.hpp"
#include "lattice.hpp"


struct EdgePrintData {
    boost::optional<Lattice::EdgeDescriptor> source;
    boost::optional<Lattice::EdgeDescriptor> parent;
    std::vector<std::string> printedElements;

    EdgePrintData(
        EdgeData & data
    ) :
        source(data.source),
        parent(data.parent)
    { }

    void addPrintedElement(std::string printedElement) {
        printedElements.push_back(printedElement);
    }

    void swap(EdgePrintData & other) {
        printedElements.swap(other.printedElements);
        boost::optional<Lattice::EdgeDescriptor> tmpSource = source;
        source = other.source;
        other.source = tmpSource;
        boost::optional<Lattice::EdgeDescriptor> tmpParent = parent;
        parent = other.parent;
        other.parent = tmpParent;
    }

    bool operator<(const EdgePrintData & other) const {
        return printedElements < other.printedElements;
    }
};


#endif
