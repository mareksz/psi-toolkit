#include "antiselector.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/assign/list_of.hpp>

#include "zvalue.hpp"


const std::string Antiselector::Factory::DEFAULT_IN_TAG = "conditional";
const std::string Antiselector::Factory::DEFAULT_FALLBACK_TAG = "token";
const std::string Antiselector::Factory::DEFAULT_OUT_TAGS = "selected,token";


Antiselector::Antiselector(
    const std::string& inTag,
    const std::string& fallbackTag,
    const std::string& outTagsSpecification,
    bool withBlank)
    : inTag_(inTag),
      fallbackTag_(fallbackTag),
      outTags_(
          LayerTagManager::splitCollectionSpecification(outTagsSpecification)),
      withBlank_(withBlank) {
}

Annotator* Antiselector::Factory::doCreateAnnotator(
    const boost::program_options::variables_map& options) {
    std::string inTag = options["in-tag"].as<std::string>();
    std::string fallbackTag = options["fallback-tag"].as<std::string>();
    std::string outTag = options["out-tags"].as<std::string>();
    bool withBlank = options.count("with-blank");

    return new Antiselector(inTag, fallbackTag, outTag, withBlank);
}

std::list<std::list<std::string> > Antiselector::Factory::doRequiredLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::list<std::string> > Antiselector::Factory::doOptionalLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::string> Antiselector::Factory::doProvidedLayerTags() {
    return std::list<std::string>();
}

void Antiselector::Factory::doAddLanguageIndependentOptionsHandled(
    boost::program_options::options_description& optionsDescription) {
    optionsDescription.add_options()
    ("in-tag", boost::program_options::value<std::string>()
     ->default_value(DEFAULT_IN_TAG),
     "tag to count")
    ("fallback-tag", boost::program_options::value<std::string>()
     ->default_value(DEFAULT_FALLBACK_TAG),
     "base tag")
    ("out-tags", boost::program_options::value<std::string>()
     ->default_value(DEFAULT_OUT_TAGS),
     "tags to mark not selected edges")
    ("with-blank",
     "do not skip edges with whitespace text");
}

AnnotatorFactory::LanguagesHandling Antiselector::Factory::doLanguagesHandling(
    const boost::program_options::variables_map& /* options */) const {

    return AnnotatorFactory::LANGUAGE_INDEPENDENT;
}

std::string Antiselector::Factory::doGetContinuation(
    const boost::program_options::variables_map& options) const {
    return std::string("simple-writer --tags ")
           + options["out-tags"].as<std::string>()
           + std::string(" --sep \" \"");
}


boost::filesystem::path Antiselector::Factory::doGetFile() const {
    return __FILE__;
}

std::string Antiselector::Factory::doGetName() const {
    return "antiselector";
}

std::string Antiselector::Factory::doGetSubType() const {
    return "converter";
}


LatticeWorker* Antiselector::doCreateLatticeWorker(Lattice & lattice) {
    return new Worker(*this, lattice);
}


Antiselector::Worker::Worker(Antiselector & processor, Lattice & lattice) :
    LatticeWorker(lattice), processor_(processor),
    outTags_(lattice_.getLayerTagManager().createTagCollection(processor_.outTags_)) {
}


void Antiselector::Worker::doRun() {
    AnnotationItemManager& aim = lattice_.getAnnotationItemManager();
    LayerTagManager& ltm = lattice_.getLayerTagManager();
    LayerTagMask fallbackMask = ltm.getMask(processor_.fallbackTag_);
    LayerTagMask inMask = ltm.getMask(processor_.inTag_);

    Lattice::EdgesSortedBySourceIterator fei
        = lattice_.edgesSortedBySource(fallbackMask);
    while (fei.hasNext()) {
        Lattice::EdgeDescriptor edge = fei.next();
        Lattice::VertexDescriptor source = lattice_.getEdgeSource(edge);
        Lattice::VertexDescriptor target = lattice_.getEdgeTarget(edge);
        Lattice::InOutEdgesIterator iei = lattice_.outEdges(source, inMask);
        AnnotationItem ai = lattice_.getEdgeAnnotationItem(edge);
        bool coedge = false;
        while (iei.hasNext()) {
            Lattice::EdgeDescriptor inEdge = iei.next();
            Lattice::VertexDescriptor inTarget = lattice_.getEdgeTarget(inEdge);
            if (inTarget == target) {
                coedge = true;
                break;
            }
        }
        if (!coedge && (processor_.withBlank_ || aim.getCategory(ai) != "B")) {
            lattice_.addEdge(
                source,
                target,
                ai,
                outTags_);
        }
    }
}


std::string Antiselector::doInfo() {
    return "selector";
}
