#include "nkjp_lattice_reader.hpp"

#include <cstring>

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>

#include "xml_property_tree.hpp"

#include "logging.hpp"


std::string NKJPLatticeReader::getFormatName() {
    return "NKJP";
}

std::string NKJPLatticeReader::doInfo() {
    return "NKJP reader";
}

NKJPLatticeReader::Factory::~Factory() { }

LatticeReader<std::istream>* NKJPLatticeReader::Factory::doCreateLatticeReader(
    const boost::program_options::variables_map& /*options*/
) {
    return new NKJPLatticeReader();
}

boost::program_options::options_description NKJPLatticeReader::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ;

    return optionsDescription;
}

std::string NKJPLatticeReader::Factory::doGetName() {
    return "nkjp-reader";
}

boost::filesystem::path NKJPLatticeReader::Factory::doGetFile() {
    return __FILE__;
}

NKJPLatticeReader::Worker::Worker(
    NKJPLatticeReader& processor,
    std::istream& inputStream,
    Lattice& lattice
) :
    ReaderWorker<std::istream>(inputStream, lattice),
    processor_(processor)
{ }

void NKJPLatticeReader::Worker::doRun() {
    SET_LOGGING_LEVEL("DEBUG");
    XmlPropertyTree xpt(inputStream_);
    BOOST_FOREACH(
        boost::property_tree::ptree::value_type &vP,
        xpt.get_child("teiCorpus.TEI.text.body")
    ) {
        if (strcmp(vP.first.data(), "p") == 0) {
            BOOST_FOREACH(
                boost::property_tree::ptree::value_type &vS,
                vP.second.get_child("")
            ) {
                if (strcmp(vS.first.data(), "s") == 0) {
                    BOOST_FOREACH(
                        boost::property_tree::ptree::value_type &vSeg,
                        vS.second.get_child("")
                    ) {
                        DEBUG(vSeg.second.get("fs.f.string", "BŁĄD"));
                    }
                }
            }
        }
    }
}
