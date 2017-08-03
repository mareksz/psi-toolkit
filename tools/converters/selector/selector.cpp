#include "selector.hpp"

#include <boost/assign/list_of.hpp>

#include "zvalue.hpp"


const std::string Selector::Factory::DEFAULT_IN_TAG = "conditional";
const std::string Selector::Factory::DEFAULT_FALLBACK_TAG = "token";
const std::string Selector::Factory::DEFAULT_OUT_TAGS = "selected";


Selector::Selector(
    const std::string& inTag,
    const std::string& fallbackTag,
    const std::string& testTag,
    const std::string& outTagsSpecification)
    : inTag_(inTag),
      fallbackTag_(fallbackTag),
      testTag_(testTag),
      outTags_(
         LayerTagManager::splitCollectionSpecification(outTagsSpecification))
{
}

Annotator* Selector::Factory::doCreateAnnotator(
    const boost::program_options::variables_map& options)
{
    std::string inTag = options["in-tag"].as<std::string>();
    std::string fallbackTag = options["fallback-tag"].as<std::string>();
    std::string testTag = options["test-tag"].as<std::string>();
    std::string outTag = options["out-tags"].as<std::string>();

    return new Selector(inTag, fallbackTag, testTag, outTag);
}

std::list<std::list<std::string> > Selector::Factory::doRequiredLayerTags()
{
    return std::list<std::list<std::string> >();
}

std::list<std::list<std::string> > Selector::Factory::doOptionalLayerTags()
{
    return std::list<std::list<std::string> >();
}

std::list<std::string> Selector::Factory::doProvidedLayerTags()
{
    return std::list<std::string>();
}

void Selector::Factory::doAddLanguageIndependentOptionsHandled(
    boost::program_options::options_description& optionsDescription)
{
    optionsDescription.add_options()
        ("in-tag", boost::program_options::value<std::string>()
         ->default_value(DEFAULT_IN_TAG),
         "tag to select when condition succeeds")
        ("fallback-tag", boost::program_options::value<std::string>()
         ->default_value(DEFAULT_FALLBACK_TAG),
         "tag to select when condition fails")
        ("test-tag", boost::program_options::value<std::string>()
         ->default_value(std::string()),
         "tag to test condition")
        ("out-tags", boost::program_options::value<std::string>()
         ->default_value(DEFAULT_OUT_TAGS),
         "tags to mark selected edges");
}

AnnotatorFactory::LanguagesHandling Selector::Factory::doLanguagesHandling(
    const boost::program_options::variables_map& /* options */) const {

    return AnnotatorFactory::LANGUAGE_INDEPENDENT;
}

std::string Selector::Factory::doGetContinuation(
    const boost::program_options::variables_map& options) const
{
    return std::string("simple-writer --tags ")
        + options["out-tags"].as<std::string>()
        + std::string(" --sep \" \"");
}


boost::filesystem::path Selector::Factory::doGetFile() const
{
    return __FILE__;
}

std::string Selector::Factory::doGetName() const
{
    return "selector";
}

std::string Selector::Factory::doGetSubType() const
{
    return "converter";
}


LatticeWorker* Selector::doCreateLatticeWorker(Lattice & lattice)
{
    return new Worker(*this, lattice);
}


Selector::Worker::Worker(Selector & processor, Lattice & lattice) :
    LatticeWorker(lattice), processor_(processor),
    outTags_(lattice_.getLayerTagManager().createTagCollection(processor_.outTags_))
{
}


void Selector::Worker::doRun()
{
    LayerTagManager& ltm = lattice_.getLayerTagManager();
    LayerTagMask fallbackMask = ltm.getMask(processor_.fallbackTag_);
    LayerTagMask inMask = ltm.getMask(processor_.inTag_);

    Lattice::EdgesSortedBySourceIterator fallbackIter(lattice_, fallbackMask);
    while (fallbackIter.hasNext())
    {
        Lattice::EdgeDescriptor fallbackEdge = fallbackIter.next();
        LayerTagCollection fallbackTags = lattice_.getEdgeLayerTags(fallbackEdge);
        if (ltm.isThere(processor_.inTag_, fallbackTags)) {
            continue;
        }

        Lattice::VertexDescriptor source = lattice_.getEdgeSource(fallbackEdge);
        Lattice::VertexDescriptor target = lattice_.getEdgeTarget(fallbackEdge);

        bool anyConditionSatisfied = false;

        Lattice::InOutEdgesIterator inIter = lattice_.outEdges(source, inMask);
        while (inIter.hasNext()) {
            // Find in-edges that are co-located with the fallback edge.
            Lattice::EdgeDescriptor inEdge = inIter.next();
            if (lattice_.getEdgeTarget(inEdge) == target
                    && isConditionSatisfied_(inEdge)) {
                anyConditionSatisfied = true;
                lattice_.addEdge(
                    source,
                    target,
                    lattice_.getEdgeAnnotationItem(inEdge),
                    outTags_);
            }
        }

        if (not anyConditionSatisfied) {
            lattice_.addEdge(
                source,
                target,
                lattice_.getEdgeAnnotationItem(fallbackEdge),
                outTags_);
        }
    }
}


bool Selector::Worker::isConditionSatisfied_(Lattice::EdgeDescriptor& edge) {
    LayerTagManager& ltm = lattice_.getLayerTagManager();
    AnnotationItemManager& aim = lattice_.getAnnotationItemManager();
    LayerTagMask testMask = processor_.testTag_.empty()
        ? ltm.anyTag()
        : ltm.getMask(processor_.testTag_);
    Lattice::VertexDescriptor source = lattice_.getEdgeSource(edge);
    Lattice::VertexDescriptor target = lattice_.getEdgeTarget(edge);

    AnnotationItem ai = lattice_.getEdgeAnnotationItem(edge);
    zvalue conditionValue = aim.getValue(ai, "condition");

    if (aim.is_false(conditionValue)) {
        // If no condition exists, it is treated as not satisfied.
        return true;
    } else {
        std::string condition = aim.to_string(conditionValue);
        Lattice::InOutEdgesIterator testIter = lattice_.outEdges(source, testMask);
        while (testIter.hasNext()) {
            Lattice::EdgeDescriptor testEdge = testIter.next();
            if (lattice_.getEdgeTarget(testEdge) == target) {
                AnnotationItem testAI = lattice_.getEdgeAnnotationItem(testEdge);
                std::string testCat = aim.getCategory(testAI);
                if (testCat == condition) {
                    return true;
                }
                std::list< std::pair<std::string, std::string> > testVals
                    = aim.getValues(testAI);
                for (std::list< std::pair<std::string, std::string> >::iterator valIter = testVals.begin();
                        valIter != testVals.end();
                        ++valIter) {
                    if (valIter->first + "=" + valIter->second == condition) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}


std::string Selector::doInfo()
{
    return "selector";
}
