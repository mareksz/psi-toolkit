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
}


LatticeWorker* SimplenormNormalizer::doCreateLatticeWorker(Lattice& lattice)
{
    return new Worker(*this, lattice);
}


SimplenormNormalizer::Worker::Worker(SimplenormNormalizer& processor, Lattice& lattice):
    LatticeWorker(lattice), processor_(processor)
{
}


void SimplenormNormalizer::Worker::doRun()
{
    DEBUG("starting simplenorm-normalizer...");
    processor_.normalize(lattice_);
}


std::string SimplenormNormalizer::doInfo()
{
    return "Simplenorm normalizer";
}


void SimplenormNormalizer::normalize(Lattice & lattice) {
    LayerTagMask maskToken = lattice.getLayerTagManager().getMaskWithLangCode(
        "token",
        langCode_
    );
    LayerTagCollection tagNormalization
        = lattice.getLayerTagManager().createTagCollectionFromListWithLangCode(
            boost::assign::list_of("simplenorm")("normalization"),
            langCode_
        );
    Lattice::EdgesSortedBySourceIterator ei(lattice, maskToken);
    while (ei.hasNext()) {
        Lattice::EdgeDescriptor edge = ei.next();
        std::string category = lattice.getAnnotationCategory(edge);
        if ("T" == category) {
            std::string text = lattice.getEdgeText(edge);
            if ("dr" == text) {
                Lattice::VertexDescriptor source = lattice.getEdgeSource(edge);
                Lattice::VertexDescriptor target = lattice.getEdgeTarget(edge);
                AnnotationItem ai("T", std::string("doktor"));
                lattice.addEdge(
                        source,
                        target,
                        ai,
                        tagNormalization);
            }
        }
    }
}
