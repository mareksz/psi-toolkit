#include "simplenorm_normalizer.hpp"

#include <cassert>
#include <boost/function_output_iterator.hpp>

#include "logging.hpp"
#include "config.hpp"

#include "annotation_item.hpp"
#include "escaping.hpp"


Annotator* SimplenormNormalizer::Factory::doCreateAnnotator(
    const boost::program_options::variables_map& options)
{
    std::string lang = options["lang"].as<std::string>();

    LangSpecificProcessorFileFetcher fileFetcher(__FILE__, lang);

    std::string rulesFileSpec = options["rules"].as<std::string>();

    boost::filesystem::path rules
        = fileFetcher.getOneFile(rulesFileSpec);

    return new SimplenormNormalizer(lang, rules);
}


void SimplenormNormalizer::Factory::doAddLanguageIndependentOptionsHandled(
    boost::program_options::options_description& optionsDescription)
{

    optionsDescription.add_options()
    ("rules",
     boost::program_options::value<std::string>()
     ->default_value(DEFAULT_RULE_FILE_SPEC),
     "rule file")
    ;

}


std::string SimplenormNormalizer::Factory::doGetName() const
{
    return "simplenorm-normalizer";
}


std::string SimplenormNormalizer::Factory::doGetSubType() const
{
    return "normalizer";
}


boost::filesystem::path SimplenormNormalizer::Factory::doGetFile() const
{
    return __FILE__;
}


std::list<std::list<std::string> > SimplenormNormalizer::Factory::doRequiredLayerTags()
{
    return boost::assign::list_of
        (boost::assign::list_of(std::string("token")));
}


std::list<std::list<std::string> > SimplenormNormalizer::Factory::doOptionalLayerTags()
{
    return std::list<std::list<std::string> >();
}


std::list<std::string> SimplenormNormalizer::Factory::doProvidedLayerTags()
{
    std::list<std::string> layerTags;
    layerTags.push_back("norm");
    return layerTags;
}


const std::string SimplenormNormalizer::Factory::DEFAULT_RULE_FILE_SPEC
    = "%ITSDATA%/%LANG%/normalization.tsv";


SimplenormNormalizer::SimplenormNormalizer(
    const std::string& lang,
    boost::filesystem::path rules):
    langCode_(lang)
{
    //TODO
}


LatticeWorker* SimplenormNormalizer::doCreateLatticeWorker(Lattice& lattice)
{
    return new Worker(*this, lattice);
}


SimplenormNormalizer::Worker::Worker(Processor& processor, Lattice& lattice):
    LatticeWorker(lattice), processor_(processor)
{
}


void SimplenormNormalizer::Worker::doRun()
{
    DEBUG("Startnig simplenorm-normalizer...");

    LayerTagMask symbolMask = lattice_.getSymbolMask();
    LayerTagMask textMask = lattice_.getLayerTagManager().getMask(
                                lattice_.getLayerTagManager()
                                .createSingletonTagCollectionWithLangCode(
                                    "text", dynamic_cast<SimplenormNormalizer&>(processor_).langCode_));

}


std::string SimplenormNormalizer::doInfo()
{
    return "Simplenorm normalizer";
}
