#include "unumsunt.hpp"


Annotator* Unumsunt::Factory::doCreateAnnotator(
    const boost::program_options::variables_map & options
) {
    std::string lang = options["lang"].as<std::string>();
    LangSpecificProcessorFileFetcher fileFetcher(__FILE__, lang);

    std::string rulesPathString;

    if (options.count("rules")) {
        std::string rulesFilename = options["rules"].as<std::string>();
        boost::filesystem::path rulesPath = fileFetcher.getOneFile(rulesFilename);
        rulesPathString = rulesPath.string();
    }

    return new Unumsunt(rulesPathString);
}


void Unumsunt::Factory::doAddLanguageIndependentOptionsHandled(
    boost::program_options::options_description& optionsDescription
) {
    optionsDescription.add_options()
        ("rules",
        boost::program_options::value<std::string>()->default_value(DEFAULT_RULE_FILE),
        "rules file");
}


std::string Unumsunt::Factory::doGetName() const {
    return "tagset-converter";
}


boost::filesystem::path Unumsunt::Factory::doGetFile() const {
    return __FILE__;
}


std::list< std::list<std::string> > Unumsunt::Factory::doRequiredLayerTags() {
    return std::list< std::list<std::string> >();
}


std::list< std::list<std::string> > Unumsunt::Factory::doOptionalLayerTags() {
    return std::list< std::list<std::string> >();
}


std::list<std::string> Unumsunt::Factory::doProvidedLayerTags() {
    std::list<std::string> layerTags;
    layerTags.push_back("tagset-converter");
    return layerTags;
}


const std::string Unumsunt::Factory::DEFAULT_RULE_FILE
    = "%ITSDATA%/%LANG%.u";


LatticeWorker* Unumsunt::doCreateLatticeWorker(Lattice & lattice) {
    return new Worker(*this, lattice);
}


Unumsunt::Worker::Worker(Unumsunt & processor, Lattice & lattice) :
    LatticeWorker(lattice), processor_(processor)
{ }


void Unumsunt::Worker::doRun() {
    // TODO
}


std::string Unumsunt::doInfo() {
    return "unumsunt tagset converter";
}


Unumsunt::Unumsunt(std::string rulesPath) : rulesPath_(rulesPath) { }
