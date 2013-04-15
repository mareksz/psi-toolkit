#include "language_dependent_annotator_factory.hpp"

#include <boost/assign/list_of.hpp>

LanguageDependentAnnotatorFactory::~LanguageDependentAnnotatorFactory() {
}

boost::program_options::options_description LanguageDependentAnnotatorFactory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription(OPTION_LABEL);

    addLanguageDependentOptions(optionsDescription);

    doAddLanguageIndependentOptionsHandled(optionsDescription);

    return optionsDescription;

}

AnnotatorFactory::LanguagesHandling LanguageDependentAnnotatorFactory::checkLangOption(
    const boost::program_options::variables_map& options) {

    return isJustNoLanguage_(options)
           ? AnnotatorFactory::LANGUAGE_DEPENDENT
           : AnnotatorFactory::JUST_ONE_LANGUAGE;
}

bool LanguageDependentAnnotatorFactory::isJustNoLanguage_(
    const boost::program_options::variables_map& options) {

    if (!options.count("lang"))
        return true;

    std::string lang = options["lang"].as<std::string>();
    return (lang == GUESS_VALUE_FOR_LANG_OPTION);
}

AnnotatorFactory::LanguagesHandling LanguageDependentAnnotatorFactory::doLanguagesHandling(
    const boost::program_options::variables_map& options) const {

    return checkLangOption(options);
}

std::list<std::string> LanguageDependentAnnotatorFactory::doLanguagesHandled(
    const boost::program_options::variables_map& options) const {

    return isJustNoLanguage_(options)
           ? doAllLanguagesHandled()
           : boost::assign::list_of(options["lang"].as<std::string>());
}

std::list<std::string> LanguageDependentAnnotatorFactory::doAllLanguagesHandled() const {
    std::vector<std::string> langs;

    boost::filesystem::path dataDirectory = getDataDirectory();

    boost::filesystem::directory_iterator end_iter;
    for (boost::filesystem::directory_iterator fiter(dataDirectory);
         fiter != end_iter;
         ++fiter) {
        if (boost::filesystem::is_directory(fiter->path())) {
            // trick for compilation with both Boost 1.42 and 1.47
            // (in Boost 1.42 filename() returns a string
            //  rather than a path)
            boost::filesystem::path seg(fiter->path().filename());

            if (seg != "m")
                langs.push_back(seg.string());
        }
    }

    std::sort(langs.begin(), langs.end());

    return std::list<std::string>(langs.begin(), langs.end());
}


void LanguageDependentAnnotatorFactory::addLanguageDependentOptions(
    boost::program_options::options_description& optionsDescription) {

    optionsDescription.add_options()
        ("lang", boost::program_options::value<std::string>()
                ->default_value(GUESS_VALUE_FOR_LANG_OPTION)->required(),
         "language")
        ("force-language", boost::program_options::bool_switch()->default_value(false),
         "force using specified language even if a text was resognised otherwise");
}

const std::string LanguageDependentAnnotatorFactory::GUESS_VALUE_FOR_LANG_OPTION = "guess";
