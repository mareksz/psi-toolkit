#include "selector.hpp"

#include <boost/assign/list_of.hpp>


Selector::Selector(
    const std::string& inputTag,
    const std::string& outputTag)
    : inputTag_(inputTag),
      outputTag_(outputTag)
{
}

Annotator* Selector::Factory::doCreateAnnotator(
    const boost::program_options::variables_map& options)
{
    std::string inTag = options["in-tag"].as<std::string>();
    std::string outTag = options["out-tag"].as<std::string>();

    return new Selector(inTag, outTag);
}

std::list<std::list<std::string> > Selector::Factory::doRequiredLayerTags()
{
    return std::list<std::list<std::string> >();
}

std::list<std::list<std::string> > Selector::Factory::doRequiredLayerTags(
    const boost::program_options::variables_map& options)
{
    return boost::assign::list_of(
            boost::assign::list_of(options["in-tag"].as<std::string>()));
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
         "tag to perform selection")
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
    LayerTagMask inputMask = tagManager.getMask(selectorProcessor.inputTag_);

    Lattice::EdgesSortedBySourceIterator ei(lattice_, inputMask);
    while (ei.hasNext())
    {
        Lattice::EdgeDescriptor edge = ei.next();
        Lattice::VertexDescriptor source = lattice_.getEdgeSource(edge);
        Lattice::VertexDescriptor target = lattice_.getEdgeTarget(edge);
        AnnotationItem ai(lattice_.getAnnotationCategory(edge));
        lattice_.addEdge(
            source,
            target,
            ai,
            outputTags_);
    }
}

std::string Selector::doInfo()
{
    return "selector";
}
