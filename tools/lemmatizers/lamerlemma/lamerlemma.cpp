#include "lamerlemma.hpp"
#include "psi_exception.hpp"

LamerLemma::LamerLemma(const boost::program_options::variables_map& options)
{
    std::string lang = options["lang"].as<std::string>();
    langCode_ = lang;

    level_ = options["level"].as<int>();

    LangSpecificProcessorFileFetcher fileFetcher(__FILE__, lang);

    if (options.count("plain-text-lexicon")) {
        if (options.count("binary-lexicon")
            && options["binary-lexicon"].as<std::string>() != DEFAULT_LAMERLEMMA_SPEC)
            throw new PsiException(
                "either --plain-text-lexicon or --binary-lexicon expected, not both");

        boost::filesystem::path plainTextLexiconPath =
            fileFetcher.getOneFile(
                options["plain-text-lexicon"].as<std::string>());

        dict_.readDictionary(plainTextLexiconPath.string());
    } else if (options.count("binary-lexicon")) {
        boost::filesystem::path binaryLexiconPath =
            fileFetcher.getOneFile(
                options["binary-lexicon"].as<std::string>());

        dict_.load(binaryLexiconPath.string());
    }

    if (options.count("save-binary-lexicon")) {
        if (dict_.isEmpty())
            throw new PsiException("no data to save");

        boost::filesystem::path binaryLexiconPath(
            options["save-binary-lexicon"].as<std::string>());

        dict_.save(binaryLexiconPath.string());
    }

}

bool LamerLemma::lemmatize(const std::string& token,
                           AnnotationItemManager& annotationItemManager,
                           LemmatizerOutputIterator& outputIterator) {

    bool foundLemma = false;

    if (level_ == 0)
        return false;
    else {
        LemmaMap lemmaMap = dict_.get(token);
        if (lemmaMap.size()) {
            if (!foundLemma)
                outputIterator.addNormalization(token);
            foundLemma = true;
        }
        else
            return false;

        BOOST_FOREACH(LemmaMap::value_type lemmaPair, lemmaMap)
        {
            std::string lemma = lemmaPair.first;
            outputIterator.addLemma(lemma);

            if (level_ > 1) {
                TagMap &tagMap = lemmaPair.second;
                BOOST_FOREACH(TagMap::value_type tagPair, tagMap) {

                    FeatureMap posFeatureMap = tagPair.second.first;
                    std::string pos = posFeatureMap["pos"];
                    std::string lexeme = lemma + LEMMA_CATEGORY_SEPARATOR + pos;

                    AnnotationItem ai_lexeme(pos, StringFrag(lexeme));
                    BOOST_FOREACH(FeatureMap::value_type feature, posFeatureMap) {
                        if (feature.first != "pos")
                            annotationItemManager.setValue(ai_lexeme, feature.first,
                                                           feature.second);
                    }
                    outputIterator.addLexeme(ai_lexeme);

                    if (level_ > 2) {
                        FeatureMapSet featureMapSet = tagPair.second.second;
                        BOOST_FOREACH(FeatureMap featureMap, featureMapSet) {
                            AnnotationItem form(pos, StringFrag(token));
                            BOOST_FOREACH(FeatureMap::value_type feature, featureMap) {
                                annotationItemManager.setValue(form, feature.first,
                                                               feature.second);
                            }
                            outputIterator.addForm(form);
                        }
                    }
                }
            }
        }
    }
    return foundLemma;
}

boost::program_options::options_description LamerLemma::optionsHandled() {
    boost::program_options::options_description desc;

    LanguageDependentAnnotatorFactory::addLanguageDependentOptions(desc);

    desc.add_options()
        ("binary-lexicon",boost::program_options::value<std::string>()
         ->default_value(DEFAULT_LAMERLEMMA_SPEC),
         "path to the lexicon in the binary format")
        ("level", boost::program_options::value<int>()->default_value(3),
         "set word processing level 0-3 (0 - do nothing, 1 - return only base forms, "
         "2 - add grammatical class and main attributes, 3 - add detailed attributes)")
        ("plain-text-lexicon",
         boost::program_options::value<std::string>(),
         "path to the lexicon in the plain text format")
        ("save-binary-lexicon",
         boost::program_options::value<std::string>(),
         "as a side effect the lexicon in the binary format is generated");

    return desc;
}


std::string LamerLemma::getName() {
    return "lamerlemma";
}

boost::filesystem::path LamerLemma::getFile() {
    return __FILE__;
}

std::string LamerLemma::getLanguage() const {
    return langCode_;
}

AnnotatorFactory::LanguagesHandling LamerLemma::languagesHandling(
    const boost::program_options::variables_map& options) {
    return LanguageDependentAnnotatorFactory::checkLangOption(options);
}

std::list<std::string> LamerLemma::languagesHandled(
    const boost::program_options::variables_map& options) {

    if (LanguageDependentAnnotatorFactory::checkLangOption(options)
        == AnnotatorFactory::JUST_ONE_LANGUAGE)
        return boost::assign::list_of(options["lang"].as<std::string>());

    std::string fileSuffix = ".bin";

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

bool LamerLemma::checkRequirements(
    const boost::program_options::variables_map& /*options*/,
    std::ostream & /*message*/) {

    return true;
}

std::list<std::string> LamerLemma::getLayerTags() {
    std::list<std::string> layerTags;

    layerTags.push_back("lamerlemma");
    layerTags.push_back("lamerlemma-tagset");

    return layerTags;
}

const std::string LamerLemma::DEFAULT_LAMERLEMMA_SPEC = "%ITSDATA%/%LANG%.bin";
