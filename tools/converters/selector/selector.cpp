#include "selector.hpp"

#include <boost/assign/list_of.hpp>


Selector::Selector(
    const std::string& inputTag,
    const std::string& testTag,
    const std::string& outputTag)
    : inputTag_(inputTag),
      testTag_(testTag),
      outputTag_(outputTag)
{
}

Annotator* Selector::Factory::doCreateAnnotator(
    const boost::program_options::variables_map& options)
{
    std::string inTag = options["in-tag"].as<std::string>();
    std::string testTag = options["test-tag"].as<std::string>();
    std::string outTag = options["out-tag"].as<std::string>();

    return new Selector(inTag, testTag, outTag);
}

std::list<std::list<std::string> > Selector::Factory::doRequiredLayerTags()
{
    return std::list<std::list<std::string> >();
}

std::list<std::list<std::string> > Selector::Factory::doRequiredLayerTags(
    const boost::program_options::variables_map& /* options */)
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

std::list<std::string> Selector::Factory::doProvidedLayerTags(
    const boost::program_options::variables_map& options)
{
    return boost::assign::list_of(options["out-tag"].as<std::string>());
}

void Selector::Factory::doAddLanguageIndependentOptionsHandled(
    boost::program_options::options_description& optionsDescription)
{
    optionsDescription.add_options()
        ("in-tag", boost::program_options::value<std::string>()
         ->required(),
         "tag to select from")
        ("test-tag", boost::program_options::value<std::string>()
         ->default_value(std::string()),
         "tag to test condition")
        ("out-tag", boost::program_options::value<std::string>()
         ->required(),
         "tag to mark selected edges");
}

AnnotatorFactory::LanguagesHandling Selector::Factory::doLanguagesHandling(
    const boost::program_options::variables_map& /* options */) const {

    return AnnotatorFactory::LANGUAGE_INDEPENDENT;
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
    outputTags_(lattice_.getLayerTagManager().createTagCollection(processor_.outputTag_))
{
}


void Selector::Worker::doRun()
{
    Selector& selectorProcessor = dynamic_cast<Selector&>(processor_);

    LayerTagManager& tagManager = lattice_.getLayerTagManager();
    AnnotationItemManager& aiManager = lattice_.getAnnotationItemManager();

    LayerTagMask inputMask = tagManager.getMask(selectorProcessor.inputTag_);
    LayerTagMask testMask = selectorProcessor.testTag_.empty()
        ? tagManager.anyTag()
        : tagManager.getMask(selectorProcessor.testTag_);

    Lattice::EdgesSortedBySourceIterator ei(lattice_, inputMask);
    while (ei.hasNext())
    {
        Lattice::EdgeDescriptor edge = ei.next();
        Lattice::VertexDescriptor source = lattice_.getEdgeSource(edge);
        Lattice::VertexDescriptor target = lattice_.getEdgeTarget(edge);
        AnnotationItem eai = lattice_.getEdgeAnnotationItem(edge);
        std::string condition = aiManager.getValueAsString(eai, "condition");

        bool conditionSatisfied = false;
        Lattice::InOutEdgesIterator ti = lattice_.outEdges(source, testMask);
        while (ti.hasNext()) {
            Lattice::EdgeDescriptor testEdge = ti.next();
            if (lattice_.getEdgeTarget(testEdge) == target) {
                AnnotationItem tai = lattice_.getEdgeAnnotationItem(testEdge);
                std::string tcat = aiManager.getCategory(tai);
                if (tcat == condition) {
                    conditionSatisfied = true;
                    break;
                }
                std::list< std::pair<std::string, std::string> > tvals
                    = aiManager.getValues(tai);
                for (std::list< std::pair<std::string, std::string> >::iterator vi = tvals.begin();
                        vi != tvals.end();
                        ++vi) {
                    if ((vi->first != "condition" && vi->second == condition)
                            || vi->first + "=" + vi->second == condition) {
                        conditionSatisfied = true;
                    }
                }
            }
        }

        if (conditionSatisfied) {
            lattice_.addEdge(
                source,
                target,
                eai,
                outputTags_);
        }
    }
}

std::string Selector::doInfo()
{
    return "selector";
}
