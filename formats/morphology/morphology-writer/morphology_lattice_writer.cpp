#include "morphology_lattice_writer.hpp"

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>

#include "lattice.hpp"


std::string MorphologyLatticeWriter::getFormatName() {
    return "morphology";
}


LatticeWriter<std::ostream>* MorphologyLatticeWriter::Factory::doCreateLatticeWriter(
    const boost::program_options::variables_map& options
) {
    return new MorphologyLatticeWriter();
}


boost::program_options::options_description MorphologyLatticeWriter::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription(OPTION_LABEL);

    optionsDescription.add_options()
        ;

    return optionsDescription;
}


std::string MorphologyLatticeWriter::Factory::doGetName() const {
    return "morphology-writer";
}


boost::filesystem::path MorphologyLatticeWriter::Factory::doGetFile() const {
    return __FILE__;
}


MorphologyLatticeWriter::MorphologyLatticeWriter()
{ }


std::string MorphologyLatticeWriter::doInfo() {
    return "Morphology writer";
}


MorphologyLatticeWriter::Worker::Worker(MorphologyLatticeWriter& processor,
                                 std::ostream& outputStream,
                                 Lattice& lattice):
    AligningWriterWorker(outputStream, lattice), processor_(processor) {
}


void MorphologyLatticeWriter::Worker::doRun() {
    setAlignments_(boost::assign::list_of(13)(26));

    Lattice::EdgesSortedByTargetIterator ei
        = lattice_.edgesSortedByTarget(lattice_.getLayerTagManager().anyTag());

    while (ei.hasNext()) {
        Lattice::EdgeDescriptor edge = ei.next();
        std::list<std::string> tagNames
            = lattice_.getLayerTagManager().getTagNames(lattice_.getEdgeLayerTags(edge));
        if (std::find(tagNames.begin(), tagNames.end(), "form") != tagNames.end()) {
            std::string edgeText = lattice_.getEdgeText(edge);

            const AnnotationItem& annotationItem = lattice_.getEdgeAnnotationItem(edge);
            std::stringstream avSs;
            avSs << annotationItem.getCategory();
            typedef std::pair<std::string, std::string> StrStrPair;
            std::list<StrStrPair> avPairs
                = lattice_.getAnnotationItemManager().getValues(annotationItem);
            BOOST_FOREACH(StrStrPair av, avPairs) {
                avSs << ":";
                avSs << av.second;
            }

            std::list<Lattice::Partition> partitions = lattice_.getEdgePartitions(edge);
            BOOST_FOREACH(Lattice::Partition partition, partitions) {
                Lattice::Partition::Iterator pei(lattice_, partition);
                while (pei.hasNext()) {
                    Lattice::EdgeDescriptor partitionEdge = pei.next();
                    const AnnotationItem& partitionAI = lattice_.getEdgeAnnotationItem(partitionEdge);
                    std::string lexeme = partitionAI.getText();
                    std::string lemma = lexeme.substr(0, lexeme.find("+"));

                    std::vector<std::string> outputCells;
                    outputCells.push_back(edgeText);
                    outputCells.push_back(lemma);
                    outputCells.push_back(avSs.str());
                    printTableRow_(outputCells);
                }
            }
        }
    }
}


MorphologyLatticeWriter::Worker::~Worker() {
}
