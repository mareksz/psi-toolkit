#ifndef BRACKETING_LATTICE_WRITER_HDR
#define BRACKETING_LATTICE_WRITER_HDR

#include <list>
#include <map>
#include <set>
#include <stack>
#include <vector>

#include <boost/foreach.hpp>

#include "aligning_writer_worker.hpp"
#include "bracket_printer.hpp"
#include "edge_data.hpp"
#include "edge_print_data.hpp"
#include "lattice_writer.hpp"
#include "lattice_writer_factory.hpp"
#include "string_helpers.hpp"


class BracketingLatticeWriter : public LatticeWriter<std::ostream> {

public:
    virtual std::string getFormatName();

    class Factory : public LatticeWriterFactory<std::ostream> {
    private:
        virtual LatticeWriter<std::ostream>* doCreateLatticeWriter(
            const boost::program_options::variables_map& options);

        virtual boost::program_options::options_description doOptionsHandled();

        virtual std::string doGetName() const;
        virtual boost::filesystem::path doGetFile() const;
    };

    BracketingLatticeWriter(
        std::string openingBracket,
        std::string closingBracket,
        std::string tagSeparator,
        std::vector<std::string> showOnlyTags,
        std::vector<std::string> filter,
        std::string avPairsSeparator,
        std::string avSeparator,
        std::vector<std::string> showAttributes,
        bool showSymbolEdges,
        bool skipBlank,
        bool collapseDuplicate,
        bool disambig
    );

    std::string getOpeningBracket() const { return openingBracket_; }
    std::string getClosingBracket() const { return closingBracket_; }
    std::string getTagSeparator() const { return tagSeparator_; }
    std::string getAVPairsSeparator() const { return avPairsSeparator_; }
    std::string getAVSeparator() const { return avSeparator_; }

    bool areSomeInFilter(std::list<std::string> & tags);
    std::set<std::string> intersectOnlyTags(std::set<std::string> tags);
    std::map<std::string, std::string> filterAttributes(std::map<std::string, std::string> avMap);

    bool isShowSymbolEdges() const { return showSymbolEdges_; }
    bool isSkipBlank() const { return skipBlank_; }
    bool isCollapseDuplicate() const { return collapseDuplicate_; }
    bool isDisambig() const { return disambig_; }

private:
    virtual std::string doInfo();

    class Worker : public AligningWriterWorker {
    public:
        Worker(BracketingLatticeWriter& processor,
               std::ostream& outputStream,
               Lattice& lattice);

        virtual void doRun();

        virtual ~Worker();
    private:
        BracketingLatticeWriter& processor_;

        EdgeData getEdgeData_(
            Lattice::EdgeDescriptor edge,
            std::string role = "");

        bool shouldBeSkipped_(Lattice::EdgeDescriptor edge);

        template <typename EdgeDataContainer, typename EdgePrintDataContainer>
        void doRun_();

        template <typename EdgeDataContainer>
        void collectEdges_(
            EdgeDataContainer & container,
            Lattice::EdgeDescriptor edge,
            zvalue role = NULL_ZVALUE);
    };

    virtual WriterWorker<std::ostream>* doCreateWriterWorker(
        std::ostream& outputStream, Lattice& lattice) {

        return new Worker(*this, outputStream, lattice);
    }

    std::string openingBracket_;
    std::string closingBracket_;
    std::string tagSeparator_;
    std::set<std::string> showOnlyTags_;
    std::set<std::string> filter_;
    std::string avPairsSeparator_;
    std::string avSeparator_;
    std::set<std::string> showAttributes_;
    bool showSymbolEdges_;
    bool skipBlank_;
    bool collapseDuplicate_;
    bool disambig_;

};


template <typename EdgeDataContainer, typename EdgePrintDataContainer>
void BracketingLatticeWriter::Worker::doRun_() {

    std::vector<std::string> patterns;
    patterns.push_back(processor_.getOpeningBracket());
    patterns.push_back(processor_.getClosingBracket());

    BracketPrinter bracketPrinter(
        patterns,
        processor_.getTagSeparator(),
        processor_.getAVPairsSeparator(),
        processor_.getAVSeparator()
    );

    std::string latticeText = lattice_.getAllText();
    size_t latticeSize = latticeText.length() + 1;

    EdgeDataContainer * * edgeStore = new EdgeDataContainer * [latticeSize];
    for (size_t i = 0; i < latticeSize; i++) {
        edgeStore[i] = new EdgeDataContainer[latticeSize];
    }

    std::string * * printedBrackets = new std::string * [latticeSize];
    for (size_t i = 0; i < latticeSize; i++) {
        printedBrackets[i] = new std::string [latticeSize];
    }

    if (processor_.isDisambig()) {

        EdgeDataContainer collectedEdges;
        Lattice::VertexDescriptor currentVertex = lattice_.getFirstVertex();
        while (true) {
            Lattice::InOutEdgesIterator ei = lattice_.allOutEdges(currentVertex);
            if (ei.hasNext()) {
                Lattice::EdgeDescriptor bestEdge = ei.next();
                while (ei.hasNext()) {
                    Lattice::EdgeDescriptor currentEdge = ei.next();
                    if (shouldBeSkipped_(currentEdge)) {
                        continue;
                    }
                    try {
                        if (lattice_.getEdgeEndIndex(currentEdge) >
                                lattice_.getEdgeEndIndex(bestEdge)) {
                            bestEdge = currentEdge;
                        }
                    } catch (WrongVertexException) { }
                }
                while (true) {
                    std::vector<Lattice::EdgeDescriptor> children
                        = lattice_.getChildren(bestEdge, lattice_.getLayerTagManager().anyTag());
                    if (children.empty() || shouldBeSkipped_(children.front())) {
                        break;
                    } else {
                        bestEdge = children.front();
                    }
                }
                collectEdges_<EdgeDataContainer>(collectedEdges, bestEdge, NULL_ZVALUE);
                currentVertex = lattice_.getEdgeTarget(bestEdge);
            } else {
                break;
            }
        }

        BOOST_FOREACH(EdgeData ed, collectedEdges) {
            try {
                int begin;
                int end;
                try {
                    begin = lattice_.getEdgeBeginIndex(*ed.source);
                } catch (WrongVertexException) {
                    begin = lattice_.getEdgeBeginIndex(*ed.parent);
                }
                try {
                    end = lattice_.getEdgeEndIndex(*ed.source);
                } catch (WrongVertexException) {
                    end = lattice_.getEdgeEndIndex(*ed.parent);
                }
                BracketPrinter::insertElementIntoContainer(edgeStore[begin][end], ed);
            } catch (WrongVertexException) { }
        }

    } else {

        Lattice::EdgesSortedBySourceIterator ei
            = lattice_.edgesSortedBySource(lattice_.getLayerTagManager().anyTag());
        while (ei.hasNext()) {
            Lattice::EdgeDescriptor edge = ei.next();
            if (shouldBeSkipped_(edge)) {
                continue;
            }
            int begin;
            int end;
            try {
                begin = lattice_.getEdgeBeginIndex(edge);
            } catch (WrongVertexException) {
                boost::optional<Lattice::EdgeDescriptor> parent = lattice_.getParent(edge);
                if (parent) {
                    begin = lattice_.getEdgeBeginIndex(*parent);
                } else {
                    throw WrongVertexException("Loose edge.");
                }
            }
            try {
                end = lattice_.getEdgeEndIndex(edge);
            } catch (WrongVertexException) {
                boost::optional<Lattice::EdgeDescriptor> parent = lattice_.getParent(edge);
                if (parent) {
                    end = lattice_.getEdgeEndIndex(*parent);
                } else {
                    throw WrongVertexException("Loose edge.");
                }
            }
            EdgeData edgeData = getEdgeData_(edge);
            BracketPrinter::insertElementIntoContainer(edgeStore[begin][end], edgeData);
        }

    }

    for (size_t i = 0; i < latticeSize; i += symbolLength(latticeText, i)) {
        for (size_t j = 0; j < latticeSize; j += symbolLength(latticeText, j)) {
            if (i < j) {
                EdgePrintDataContainer printed = bracketPrinter.print(edgeStore[i][j]);
                std::vector<EdgePrintData> vprinted(printed.begin(), printed.end());
                for (size_t a = 0; a < vprinted.size(); ++a) {
                    for (size_t b = a + 1; b < vprinted.size(); ++b) {
                        if (
                            vprinted[a].source &&
                            vprinted[b].parent &&
                            *(vprinted[a].source) == *(vprinted[b].parent)
                        ) {
                            vprinted[a].swap(vprinted[b]);
                            a = 0;
                            b = 1;
                        }
                    }
                }
                BOOST_FOREACH(EdgePrintData epd, vprinted) {
                    printedBrackets[i][j] = printedBrackets[i][j] + epd.printedElements[0];
                    printedBrackets[j][i] = epd.printedElements[1] + printedBrackets[j][i];
                }
            }
        }
    }

    for (size_t i = 0; i < latticeSize; i += symbolLength(latticeText, i)) {
        for (
            size_t j = ((i + latticeSize) - 1) % latticeSize;
            j != i;
            j = ((j + latticeSize) - 1) % latticeSize
        ) {
            print_(printedBrackets[i][j]);
        }
        print_(latticeText.substr(i, symbolLength(latticeText, i)));
    }

    for (size_t i = 0; i < latticeSize; i++) {
        delete [] printedBrackets[i];
    }
    delete [] printedBrackets;

    for (size_t i = 0; i < latticeSize; i++) {
        delete [] edgeStore[i];
    }
    delete [] edgeStore;

}


template <typename EdgeDataContainer>
void BracketingLatticeWriter::Worker::collectEdges_(
    EdgeDataContainer & container,
    Lattice::EdgeDescriptor edge,
    zvalue role
) {
    if (!shouldBeSkipped_(edge)) {
        EdgeData edgeData = getEdgeData_(
            edge,
            NULLP(role) ? "" : (lattice_.getAnnotationItemManager().to_string(role)));
        BracketPrinter::insertElementIntoContainer(container, edgeData);
    }
    std::list<Lattice::Partition> partitions = lattice_.getEdgePartitions(edge);
    if (!partitions.empty()) {
        Lattice::Partition::Iterator ei(lattice_, partitions.front());
        while (ei.hasNext()) {
            Lattice::EdgeUsage eu = ei.nextUsage();
            collectEdges_(container, eu.getEdge(), eu.getRole());
        }
    }
}


#endif
