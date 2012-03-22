#include "nkjp_lattice_reader.hpp"

#include <cstring>
#include <locale>

#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "string_frag.hpp"

#include "logging.hpp"


std::string NKJPLatticeReader::getFormatName() {
    return "NKJP";
}

std::string NKJPLatticeReader::doInfo() {
    return "NKJP reader";
}

NKJPLatticeReader::Factory::~Factory() { }

LatticeReader<std::istream>* NKJPLatticeReader::Factory::doCreateLatticeReader(
    const boost::program_options::variables_map& options
) {
    return new NKJPLatticeReader(
        options["layer"].as<std::string>()
    );
}

boost::program_options::options_description NKJPLatticeReader::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ("layer", boost::program_options::value<std::string>()->default_value("morphosyntax"),
            "specifies the annotation layer represented by the given NKJP file")
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
    // SET_LOGGING_LEVEL("DEBUG");
    boost::property_tree::ptree xpt;
    boost::property_tree::read_xml(inputStream_, xpt);
    int prevEnding = 0;
    BOOST_FOREACH(
        boost::property_tree::ptree::value_type &vP,
        xpt.get_child("teiCorpus.TEI.text.body")
    ) {
        if (strcmp(vP.first.data(), "p") == 0) {
            Lattice::VertexDescriptor pBegin = lattice_.getLastVertex();
            Lattice::EdgeSequence::Builder pBuilder(lattice_);
            BOOST_FOREACH(
                boost::property_tree::ptree::value_type &vS,
                vP.second.get_child("")
            ) {
                if (strcmp(vS.first.data(), "s") == 0) {
                    Lattice::VertexDescriptor sBegin = lattice_.getLastVertex();
                    Lattice::EdgeSequence::Builder sBuilder(lattice_);
                    BOOST_FOREACH(
                        boost::property_tree::ptree::value_type &vSeg,
                        vS.second.get_child("")
                    ) {
                        if (strcmp(vSeg.first.data(), "seg") == 0) {
                            bool insertSpace = false;
                            try {
                                std::string comment(
                                    vSeg.second.get<std::string>("fs.<xmlcomment>")
                                );
                                NKJPMorphosyntaxCommentGrammar grammar;
                                NKJPMorphosyntaxCommentItem item;
                                std::string::const_iterator begin = comment.begin();
                                std::string::const_iterator end = comment.end();
                                if (parse(begin, end, grammar, item)) {
                                    if (item.beginning - prevEnding > 0) {
                                        insertSpace = true;
                                    } else {
                                        insertSpace = false;
                                    }
                                    prevEnding = item.beginning + item.length;
                                } else {
                                    insertSpace = false;
                                }
                            } catch (boost::property_tree::ptree_error) {
                                insertSpace = false;
                            }
                            try {
                                std::string segment(
                                    vSeg.second.get<std::string>("fs.f.string")
                                );
                                Lattice::EdgeDescriptor segEdge
                                    = appendSegmentToLattice_(segment, insertSpace);
                                sBuilder.addEdge(segEdge);
                            } catch (boost::property_tree::ptree_error) {
                                throw PsiException("nkjp-reader: input file error");
                            }
                        }
                    }
                    Lattice::VertexDescriptor sEnd = lattice_.getLastVertex();
                    AnnotationItem sItem(
                        "s",
                        StringFrag(lattice_.getAllText(), sBegin, sEnd-sBegin)
                    );
                    Lattice::EdgeDescriptor sEdge = lattice_.addEdge(
                        sBegin,
                        sEnd,
                        sItem,
                        getTags_("sentence"),
                        sBuilder.build()
                    );
                    pBuilder.addEdge(sEdge);
                }
            }
            Lattice::VertexDescriptor pEnd = lattice_.getLastVertex();
            AnnotationItem pItem(
                "p",
                StringFrag(lattice_.getAllText(), pBegin, pEnd-pBegin)
            );
            lattice_.addEdge(
                pBegin,
                pEnd,
                pItem,
                getTags_("paragraph"),
                pBuilder.build()
            );
        }
    }
}


LayerTagCollection NKJPLatticeReader::Worker::getTags_(std::string mainTag) {
    return lattice_.getLayerTagManager().createTagCollectionFromList(
        boost::assign::list_of("nkjp-reader")(mainTag.c_str())
    );
}


Lattice::EdgeDescriptor NKJPLatticeReader::Worker::appendSegmentToLattice_(
    std::string segment,
    bool insertSpace
) {
    if (insertSpace) {
        Lattice::VertexDescriptor sFrom = lattice_.getLastVertex();
        lattice_.appendStringWithSymbols(" ");
        Lattice::VertexDescriptor sTo = lattice_.getLastVertex();
        std::string blank(" ");
        AnnotationItem sItem("B", blank);
        Lattice::EdgeSequence::Builder sBuilder(lattice_);
        sBuilder.addEdge(lattice_.firstOutEdge(
            sFrom,
            lattice_.getLayerTagManager().getMask("symbol")
        ));
        lattice_.addEdge(sFrom, sTo, sItem, getTags_("token"), sBuilder.build());
    }

    Lattice::VertexDescriptor from = lattice_.getLastVertex();
    lattice_.appendStringWithSymbols(segment);
    Lattice::VertexDescriptor to = lattice_.getLastVertex();
    AnnotationItem item("seg", segment);
    Lattice::EdgeSequence::Builder seqBuilder(lattice_);
    Lattice::VertexDescriptor currentVertex = from;
    Lattice::EdgeDescriptor currentEdge;
    while (currentVertex != to) {
        currentEdge = lattice_.firstOutEdge(
            currentVertex,
            lattice_.getLayerTagManager().getMask("symbol")
        );
        seqBuilder.addEdge(currentEdge);
        currentVertex = lattice_.getEdgeTarget(currentEdge);
    }
    return lattice_.addEdge(from, to, item, getTags_("token"), seqBuilder.build());
}