#include "psi_lattice_writer.hpp"

#include <iomanip>
#include <iostream>

#include <boost/assign/list_of.hpp>

#include "logging.hpp"

std::string PsiLatticeWriter::getFormatName() {
    return "PSI";
}

LatticeWriter<std::ostream>* PsiLatticeWriter::Factory::doCreateLatticeWriter(
    const boost::program_options::variables_map& options
) {
    return new PsiLatticeWriter(
        !options.count("no-header")
    );
}

boost::program_options::options_description PsiLatticeWriter::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ("no-header",
            "do not print the column description");

    return optionsDescription;
}


std::string PsiLatticeWriter::Factory::doGetName() const {
    return "psi-writer";
}

boost::filesystem::path PsiLatticeWriter::Factory::doGetFile() const {
    return __FILE__;
}


std::string PsiLatticeWriter::doInfo() {
    return "PSI writer";
}

PsiLatticeWriter::Worker::Worker(PsiLatticeWriter& processor,
                                 std::ostream& outputStream,
                                 Lattice& lattice):
    AligningWriterWorker(outputStream, lattice), processor_(processor) {
}

void PsiLatticeWriter::Worker::doRun() {

    DEBUG("starting writer...");

    PsiQuoter quoter;

    std::map<Lattice::EdgeDescriptor, int> edgeOrdinalMap;

    Lattice::EdgesSortedByTargetIterator ei
        = lattice_.edgesSortedByTarget(lattice_.getLayerTagManager().anyTag());

    int ordinal = 0;

    setAlignments_(boost::assign::list_of(2)(7)(13)(26)(48)(60));

    if (processor_.isWithHeader()) {
        printTableRow_(boost::assign::list_of
            ("##")
            ("beg.")
            ("len.")
            ("text")
            ("tags")
            ("annot.text")
            ("annotations"));
    }

    std::string latticeTextCovered;

    while (ei.hasNext()) {

        Lattice::EdgeDescriptor edge = ei.next();
        Lattice::VertexDescriptor source = lattice_.getEdgeSource(edge);
        Lattice::VertexDescriptor target = lattice_.getEdgeTarget(edge);

        if (lattice_.isEdgeHidden(edge)) continue;

        if (!lattice_.isLooseVertex(source) && !lattice_.isLooseVertex(target)) {
            size_t i = latticeTextCovered.length();
            while (i < lattice_.getVertexRawCharIndex(source)) {
                try {

                    std::vector<std::string> outputCells;

                    Lattice::EdgeDescriptor rawEdge = lattice_.firstOutEdge(
                        lattice_.getVertexForRawCharIndex(i),
                        lattice_.getLayerTagManager().getMask("symbol")
                    );

                    ++ordinal;
                    edgeOrdinalMap[rawEdge] = ordinal;
                    std::stringstream ordinalSs;
                    ordinalSs << std::right << std::setfill('0') << std::setw(2);
                    ordinalSs << ordinal;
                    outputCells.push_back(ordinalSs.str());

                    std::stringstream beginningSs;
                    beginningSs << std::right << std::setfill('0') << std::setw(4);
                    beginningSs << i;
                    outputCells.push_back(beginningSs.str());

                    std::stringstream lengthSs;
                    lengthSs << std::right << std::setfill('0') << std::setw(2);
                    lengthSs << lattice_.getEdgeLength(rawEdge);
                    outputCells.push_back(lengthSs.str());

                    std::string edgeText = quoter.escape(lattice_.getEdgeText(rawEdge));
                    outputCells.push_back(edgeText);

                    outputCells.push_back("symbol");

                    const AnnotationItem& annotationItem = lattice_.getEdgeAnnotationItem(rawEdge);
                    outputCells.push_back(quoter.escape(annotationItem.getText()));

                    outputCells.push_back(quoter.escape(annotationItem.getCategory()));

                    printTableRow_(outputCells);

                    latticeTextCovered += edgeText;

                    i = lattice_.getEdgeEndIndex(rawEdge);

                } catch (NoEdgeException) {

                    // Lattice contains some text not covered by any edge.

                    std::vector<std::string> outputCells;

                    char uncoveredSymbol = lattice_.getAllText()[i];

                    ++ordinal;
                    std::stringstream ordinalSs;
                    ordinalSs << std::right << std::setfill('0') << std::setw(2);
                    ordinalSs << ordinal;
                    outputCells.push_back(ordinalSs.str());

                    std::stringstream beginningSs;
                    beginningSs << std::right << std::setfill('0') << std::setw(4);
                    beginningSs << i;
                    outputCells.push_back(beginningSs.str());

                    outputCells.push_back("01");

                    outputCells.push_back(quoter.escape(std::string(1, uncoveredSymbol)));

                    outputCells.push_back("∅");

                    outputCells.push_back("∅");

                    outputCells.push_back("∅");

                    printTableRow_(outputCells);

                    latticeTextCovered += uncoveredSymbol;
                    i++;

                }
            }
        }

        ++ordinal;

        edgeOrdinalMap[edge] = ordinal;

        std::vector<std::string> outputCells;

        // ordinal:

        std::stringstream ordinalSs;
        ordinalSs << std::right << std::setfill('0') << std::setw(2);
        ordinalSs << ordinal;
        outputCells.push_back(ordinalSs.str());

        // beginning:

        std::stringstream beginningSs;
        if (lattice_.isLooseVertex(source)) {
            beginningSs << "@" << lattice_.getLooseVertexIndex(source);
        } else {
            beginningSs << std::right << std::setfill('0') << std::setw(4);
            beginningSs << lattice_.getVertexRawCharIndex(source);
        }
        outputCells.push_back(beginningSs.str());

        // length:

        std::stringstream lengthSs;
        if (lattice_.isLooseVertex(target)) {
            lengthSs << "*@" << lattice_.getLooseVertexIndex(target);
        } else if (lattice_.isLooseVertex(source)) {
            lengthSs << "*";
            lengthSs << std::right << std::setfill('0') << std::setw(4);
            lengthSs << lattice_.getVertexRawCharIndex(target);
        } else {
            lengthSs << std::right << std::setfill('0') << std::setw(2);
            lengthSs << lattice_.getEdgeLength(edge);
        }
        outputCells.push_back(lengthSs.str());

        // (some preparations):

        const AnnotationItem& annotationItem = lattice_.getEdgeAnnotationItem(edge);
        std::string edgeText;
        if (lattice_.isLooseVertex(source) || lattice_.isLooseVertex(target)) {
            edgeText = quoter.escape(annotationItem.getText());
        } else {
            edgeText = quoter.escape(lattice_.getEdgeText(edge));
        }

        std::list<Lattice::Partition> partitions = lattice_.getEdgePartitions(edge);
        bool writeWholeText = false;
        if (
            !lattice_.isLooseVertex(source) &&
            !lattice_.isLooseVertex(target) &&
            latticeTextCovered.length() < lattice_.getVertexRawCharIndex(target)
        ) {
            writeWholeText = true;
        }

        // edge text:

        std::stringstream edgeTextSs;
        unsigned int edgeTextLength = utf8::distance(edgeText.begin(), edgeText.end());
        std::string edgeTextPrinted;
        if (edgeTextLength == 0) {
            edgeTextSs << "∅";
        } else if (edgeTextLength > getColumnWidth_(3) && !writeWholeText) {
            std::string::const_iterator bIter = edgeText.begin();
            utf8::unchecked::advance(bIter, getColumnWidth_(3) - 7);
            edgeTextSs << edgeText.substr(0, bIter - edgeText.begin()) << "...";

            std::string::const_iterator eIter = edgeText.end();
            for (int i = 0; i < 3; ++i)
                utf8::unchecked::prior(eIter);

            edgeTextPrinted = edgeText.substr(eIter - edgeText.begin());
        } else {
            edgeTextPrinted = edgeText;
        }
        if (!lattice_.isLooseVertex(source) && !lattice_.isLooseVertex(target)) {
            try {
                latticeTextCovered +=
                    edgeTextPrinted.substr(latticeTextCovered.length()
                        - lattice_.getVertexRawCharIndex(source));
            } catch (std::out_of_range) { }
        }
        edgeTextSs << edgeTextPrinted;
        outputCells.push_back(edgeTextSs.str());

        // tags:

        std::string tagStr = "";
        std::list<std::string> tagNames
            = lattice_.getLayerTagManager().getTagNames(lattice_.getEdgeLayerTags(edge));
        BOOST_FOREACH(std::string tagName, tagNames) {
            if (!tagStr.empty()) {
                tagStr += ",";
            }
            tagStr += tagName;
        }
        outputCells.push_back(quoter.escape(tagStr));

        // annotation text:

        outputCells.push_back(quoter.escape(annotationItem.getText()));

        // annotations:

        std::stringstream aiSs;

        aiSs << quoter.escape(annotationItem.getCategory());

        Lattice::Score score = lattice_.getEdgeScore(edge);
        if (score != 0.0) {
            aiSs << "<" << score << ">";
        }

        std::string avStr = "";
        typedef std::pair<std::string, std::string> StrStrPair;
        std::list<StrStrPair> avPairs
            = lattice_.getAnnotationItemManager().getValues(annotationItem);
        BOOST_FOREACH(StrStrPair av, avPairs) {
            avStr += ",";
            avStr += av.first;
            avStr += "=";
            avStr += av.second;
        }
        aiSs << quoter.escape(avStr);

        bool isDefaultPartition = (
            lattice_.getEdgeLayerTags(edge)
                == lattice_.getLayerTagManager().createSingletonTagCollection("symbol")
        );
        bool firstPartition = true;
        bool partitionBeginning = false;
        std::stringstream partSs;
        BOOST_FOREACH(Lattice::Partition partition, partitions) {
            if (firstPartition) {
                firstPartition = false;
            } else {
                partSs << ",";
            }
            std::stringstream linkSs;
            partitionBeginning = true;
            Lattice::Partition::Iterator ei(lattice_, partition);
            while (ei.hasNext()) {
                Lattice::EdgeUsage ed = ei.nextUsage();
                if (partitionBeginning) {
                    if (
                        lattice_.isEdgeHidden(ed.getEdge())
                    ) {
                        isDefaultPartition = true;
                    }
                    partitionBeginning = false;
                } else {
                    if (
                        !lattice_.isEdgeHidden(ed.getEdge())
                    ) {
                        isDefaultPartition = false;
                    }
                    linkSs << "-";
                }
                std::map<Lattice::EdgeDescriptor, int>::iterator mi
                    = edgeOrdinalMap.find(ed.getEdge());
                if (mi != edgeOrdinalMap.end()) {
                    linkSs << mi->second;

                    if (!NULLP(ed.getRole()))
                        linkSs << '$'
                            << quoter.escape(
                                lattice_.getAnnotationItemManager().zvalueToString(ed.getRole()))
                            << '$';
                } else if (!lattice_.getLayerTagManager().isThere(
                        "symbol", lattice_.getEdgeLayerTags(ed.getEdge()))) {

                    // TODO

                }
            }
            partSs << linkSs.str();
            std::list<std::string> partitionTagNames
                = lattice_.getLayerTagManager().getTagNames(partition.getTagList());
            if (partitionTagNames != tagNames) {
                std::string partitionTagStr = "";
                BOOST_FOREACH(std::string partitionTagName, partitionTagNames) {
                    if (!partitionTagStr.empty()) {
                        partitionTagStr += ",";
                    }
                    partitionTagStr += partitionTagName;
                }
                partSs << "(" << partitionTagStr << ")";
            }
            Lattice::Score partitionScore = partition.getScore();
            if (partitionScore != score) {
                partSs << "<" << partitionScore << ">";
            }
        }
        if (!isDefaultPartition) {
            aiSs << "[" << partSs.str() << "]";
        }

        outputCells.push_back(aiSs.str());

        printTableRow_(outputCells);

    }

    DEBUG("WRITING");
}

PsiLatticeWriter::Worker::~Worker() {
}
