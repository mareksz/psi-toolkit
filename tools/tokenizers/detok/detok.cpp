#include "detok.hpp"

Annotator* Detok::Factory::doCreateAnnotator(
    const boost::program_options::variables_map& options) {

    std::string lang = options["lang"].as<std::string>();
    LangSpecificProcessorFileFetcher fileFetcher(__FILE__, lang);

    Detok *detok = new Detok();

    return detok;
}

void Detok::Factory::doAddLanguageIndependentOptionsHandled(
    boost::program_options::options_description& optionsDescription) {
}

std::string Detok::Factory::doGetName() const {
    return "detok";
}

boost::filesystem::path Detok::Factory::doGetFile() const {
    return __FILE__;
}

std::list<std::list<std::string> > Detok::Factory::doRequiredLayerTags() {
    return boost::assign::list_of(
        boost::assign::list_of(std::string("form")));
}

std::list<std::list<std::string> > Detok::Factory::doOptionalLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::string> Detok::Factory::doProvidedLayerTags() {
    std::list<std::string> layerTags;
    layerTags.push_back("ttoken");
    return layerTags;
}

LatticeWorker* Detok::doCreateLatticeWorker(Lattice& lattice) {
    return new Worker(*this, lattice);
}

Detok::Worker::Worker(Detok& processor, Lattice& lattice):
    LatticeWorker(lattice), processor_(processor) {
}

void Detok::Worker::doRun() {
    INFO("detokenizer working");

    LayerTagMask mask = lattice_.getLayerTagManager().getMaskWithLangCode("form", "en");

    Lattice::EdgesSortedByTargetIterator iter(lattice_, mask);

    while (iter.hasNext()) {
        Lattice::EdgeDescriptor edge = iter.next();

        INFO("OUTPUT: " << lattice_.getAnnotationText(edge));
    }
}

std::string Detok::doInfo() {
    return "detokenizer";
}

Detok::Detok() {
}
