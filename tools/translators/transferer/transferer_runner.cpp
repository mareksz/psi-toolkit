#include "transferer_runner.hpp"
#include "config.hpp"

#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/bind.hpp>

#include "bi_lang_specific_processor_file_fetcher.hpp"
#include "bi_language_dependent_annotator_factory.hpp"
#include "its_data.hpp"

TransfererRunner::TransfererRunner(const boost::program_options::variables_map& options) {
    std::string lang = options["lang"].as<std::string>();
    langCode_ = lang;

    std::string trg_lang = options["trg-lang"].as<std::string>();

    createTags_(trg_lang);

    BiLangSpecificProcessorFileFetcher fileFetcher(__FILE__, lang, trg_lang);
}

std::string TransfererRunner::getName() {
    return "transferer";
}

boost::filesystem::path TransfererRunner::getFile() {
    return __FILE__;
}

AnnotatorFactory::LanguagesHandling TransfererRunner::languagesHandling(
    const boost::program_options::variables_map& options) {

    return LanguageDependentAnnotatorFactory::checkLangOption(options);
}

std::list<std::string> TransfererRunner::languagesHandled(
    const boost::program_options::variables_map& options) {

    if (LanguageDependentAnnotatorFactory::checkLangOption(options)
        == AnnotatorFactory::JUST_ONE_LANGUAGE)
        return boost::assign::list_of(options["lang"].as<std::string>());

    std::string trgLang = options["trg-lang"].as<std::string>();

    std::string fileSuffix = trgLang + ".mti";

    std::vector<std::string> langs;

    boost::filesystem::path dataDirectory = getItsData(getFile());

    boost::filesystem::directory_iterator end_iter;
    for (boost::filesystem::directory_iterator fiter(dataDirectory);
         fiter != end_iter;
         ++fiter) {
            boost::filesystem::path seg(fiter->path().filename());
            std::string lexiconFileName = seg.string();

            if (lexiconFileName.length() > fileSuffix.length()
                && lexiconFileName.substr(
                    lexiconFileName.length() - fileSuffix.length())
                == fileSuffix)
                langs.push_back(lexiconFileName.substr(
                                    0, lexiconFileName.length() - fileSuffix.length()));
    }

    std::sort(langs.begin(), langs.end());

    return std::list<std::string>(langs.begin(), langs.end());
}

boost::program_options::options_description TransfererRunner::optionsHandled() {
    boost::program_options::options_description desc("Allowed options");

    BiLanguageDependentAnnotatorFactory::addBiLanguageDependentOptions(desc);

    desc.add_options()
        ("rules", boost::program_options::value<std::string>()
         ->default_value(DEFAULT_RULE_FILE), "rules file");

    return desc;
}

std::list<std::string> TransfererRunner::providedLayerTags() {
    return boost::assign::list_of
        (std::string("transfered"))
        (std::string("!translation"));
}

std::list<std::list<std::string> > TransfererRunner::requiredLayerTags() {
    return
        boost::assign::list_of(
            boost::assign::list_of(std::string("parse")));
}

std::list<std::string> TransfererRunner::tagsToOperateOn() {
    return boost::assign::list_of
        (std::string("parse"))(LayerTagManager::getLanguageTag(langCode_));
}

void TransfererRunner::processEdge(Lattice& lattice, Lattice::EdgeDescriptor edge) {
    std::string edgeText = lattice.getAnnotationText(edge);

    std::cerr << "PROCESSING:" << edgeText << std::endl;
}

void TransfererRunner::createTags_(const std::string& trg_lang) {
    tags_ = providedLayerTags();
    tags_.push_back(LayerTagManager::getLanguageTag(trg_lang));
}

const std::string TransfererRunner::DEFAULT_RULE_FILE
= "%ITSDATA%/%LANG%%TRGLANG%.mti";
