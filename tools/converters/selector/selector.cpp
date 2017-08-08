#include "selector.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/assign/list_of.hpp>

#include "zvalue.hpp"


const std::string Selector::Factory::DEFAULT_IN_TAG = "conditional";
const std::string Selector::Factory::DEFAULT_FALLBACK_TAG = "token";
const std::string Selector::Factory::DEFAULT_OUT_TAGS = "selected,token";


Selector::Selector(
    const std::string& inTag,
    const std::string& fallbackTag,
    const std::string& testTag,
    const std::string& outTagsSpecification,
    bool withBlank)
    : inTag_(inTag),
      fallbackTag_(fallbackTag),
      testTag_(testTag),
      outTags_(
         LayerTagManager::splitCollectionSpecification(outTagsSpecification)),
      withBlank_(withBlank)
{
}

Annotator* Selector::Factory::doCreateAnnotator(
    const boost::program_options::variables_map& options)
{
    std::string inTag = options["in-tag"].as<std::string>();
    std::string fallbackTag = options["fallback-tag"].as<std::string>();
    std::string testTag = options["test-tag"].as<std::string>();
    std::string outTag = options["out-tags"].as<std::string>();
    bool withBlank = options.count("with-blank");

    return new Selector(inTag, fallbackTag, testTag, outTag, withBlank);
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
         "tags to mark selected edges")
        ("with-blank",
         "do not skip edges with whitespace text");
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
    AnnotationItemManager& aim = lattice_.getAnnotationItemManager();
    LayerTagManager& ltm = lattice_.getLayerTagManager();
    LayerTagMask fallbackMask = ltm.getMask(processor_.fallbackTag_);
    LayerTagMask inMask = ltm.getMask(processor_.inTag_);

    Lattice::VertexDescriptor vertex = lattice_.getFirstVertex();
    Lattice::VertexDescriptor end = lattice_.getLastVertex();
    while (vertex != end) {
        std::list<Lattice::EdgeSpec> edgesToAdd;
        bool allConditionsSatisfied = true;
        Lattice::EdgeDescriptor fallbackEdge;
        try {
            fallbackEdge = lattice_.firstOutEdge(vertex, fallbackMask);
        } catch (NoEdgeException) {
            try {
                vertex = lattice_.getEdgeTarget(lattice_.firstOutEdge(
                            vertex, lattice_.getSymbolMask()));
            } catch (NoEdgeException) {
                ++vertex;
            }
            continue;
        }
        Lattice::VertexDescriptor fallbackTarget = lattice_.getEdgeTarget(fallbackEdge);
        Lattice::VertexDescriptor inSource = vertex;
        Lattice::VertexDescriptor inTarget = vertex;

        while (inTarget != fallbackTarget) {
            Lattice::InOutEdgesIterator inIter = lattice_.outEdges(inSource, inMask);
            bool oneConditionSatisfied = false;
            while (inIter.hasNext()) {
                Lattice::EdgeDescriptor inEdge = inIter.next();
                inTarget = lattice_.getEdgeTarget(inEdge);
                if (isConditionSatisfied_(inEdge)) {
                    oneConditionSatisfied = true;
                    edgesToAdd.push_back(Lattice::EdgeSpec(
                        inSource,
                        inTarget,
                        lattice_.getEdgeAnnotationItem(inEdge),
                        outTags_));
                }
            }
            if (!oneConditionSatisfied) {
                allConditionsSatisfied = false;
            }
            if (inSource == inTarget) {
                break;
            }
            inSource = inTarget;
        }

        if (allConditionsSatisfied) {
            for (std::list<Lattice::EdgeSpec>::iterator esIter = edgesToAdd.begin();
                    esIter != edgesToAdd.end();
                    ++esIter) {
                if (processor_.withBlank_
                        || aim.getCategory(esIter->annotationItem) != "B") {
                    lattice_.addEdge(
                            esIter->fromSpec,
                            esIter->toSpec,
                            esIter->annotationItem,
                            esIter->tags);
                }
            }
        } else {
            edgesToAdd.clear();
            if (processor_.withBlank_
                    || lattice_.getAnnotationCategory(fallbackEdge) != "B") {
                lattice_.addEdge(
                    vertex,
                    fallbackTarget,
                    lattice_.getEdgeAnnotationItem(fallbackEdge),
                    outTags_);
            }
        }
        vertex = fallbackTarget;
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
        std::string conditionString = aim.to_string(conditionValue);
        if (conditionString == "*") {
            return true;
        }

        Lattice::InOutEdgesIterator testIter = lattice_.outEdges(source, testMask);
        while (testIter.hasNext()) {
            Lattice::EdgeDescriptor testEdge = testIter.next();
            if (lattice_.getEdgeTarget(testEdge) == target) {
                AnnotationItem testAI = lattice_.getEdgeAnnotationItem(testEdge);
                std::string testCat = aim.getCategory(testAI);
                std::list< std::pair<std::string, std::string> > testVals
                    = aim.getValues(testAI);

                std::vector<std::string> conditionAlts;
                boost::split(conditionAlts, conditionString, boost::is_any_of("|"));
                for (std::vector<std::string>::iterator i = conditionAlts.begin();
                        i != conditionAlts.end();
                        ++i) {
                    bool allConditionsSatisfied = true;

                    std::vector<std::string> conditions;
                    boost::split(conditions, *i, boost::is_any_of("&"));
                    for (std::vector<std::string>::iterator j = conditions.begin();
                            j != conditions.end();
                            ++j) {

                        bool conditionSatisfied = false;
                        if (testCat == *j) {
                            conditionSatisfied = true;
                        }
                        for (std::list< std::pair<std::string, std::string> >::iterator valIter = testVals.begin();
                                valIter != testVals.end();
                                ++valIter) {
                            if (valIter->first + "=" + valIter->second == *j) {
                                conditionSatisfied = true;
                                break;
                            }
                        }
                        if (!conditionSatisfied) {
                            allConditionsSatisfied = false;
                            break;
                        }
                    }

                    if (allConditionsSatisfied) {
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
