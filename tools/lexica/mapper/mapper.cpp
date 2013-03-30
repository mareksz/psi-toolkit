#include "mapper.hpp"
#include "config.hpp"

#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/bind.hpp>

#include "lang_specific_processor_file_fetcher.hpp"
#include "language_dependent_annotator_factory.hpp"

Mapper::Mapper(const boost::program_options::variables_map& options) {
    
    std::string lang = options["lang"].as<std::string>();
    langCode_ = lang;
    
    LangSpecificProcessorFileFetcher fileFetcher(__FILE__, lang);
    
    if(options.count("in-tags") > 0 && options.count("out-tags") > 0) {
        std::string inTagsString = options["in-tags"].as<std::string>();
        std::string outTagsString = options["out-tags"].as<std::string>();
        
        boost::split(inTags_, inTagsString, boost::is_any_of(","));
        boost::split(outTags_, outTagsString, boost::is_any_of(","));
    }
    else {
        throw Exception("missing --in-tags or --out-tags");
    }
    
    //if(options.count("plain-text-lexicon") == 0
    //   && options.count("binary-lexicon") == 0) {
    //    throw Exception("no mapper lexicon file given"); 
    //}
    //else {
    //    if (options.count("plain-text-lexicon") > 0) {
    //        if (options.count("binary-lexicon") > 0)
    //            throw new Exception(
    //                "either --plain-text-lexicon or --binary-lexicon expected, not both");
    //    
    //        boost::filesystem::path plainTextLexiconPath =
    //            fileFetcher.getOneFile(
    //                options["plain-text-lexicon"].as<std::string>());
    //    
    //        readPlainText_(plainTextLexiconPath);
    //    } else if (options.count("binary-lexicon") > 0) {
    //        boost::filesystem::path binaryLexiconPath =
    //            fileFetcher.getOneFile(
    //                options["binary-lexicon"].as<std::string>());
    //    
    //        loadBinary_(binaryLexiconPath);
    //    }
    //    
    //    if (options.count("save-binary-lexicon") > 0) {
    //        if (lexiconBase_.isEmpty())
    //            throw new Exception("no data to save");
    //    
    //        boost::filesystem::path binaryLexiconPath(
    //            options["save-binary-lexicon"].as<std::string>());
    //    
    //        saveBinary_(binaryLexiconPath);
    //    }
    //}
}

std::string Mapper::getName() {
    return "mapper";
}

boost::filesystem::path Mapper::getFile() {
    return __FILE__;
}

AnnotatorFactory::LanguagesHandling Mapper::languagesHandling(
    const boost::program_options::variables_map& options) {

    return LanguageDependentAnnotatorFactory::checkLangOption(options);
}

std::list<std::string> Mapper::languagesHandled(
    const boost::program_options::variables_map& options) {

    std::vector<std::string> langs;
    langs.push_back("pl");

    return std::list<std::string>(langs.begin(), langs.end());
}

boost::program_options::options_description Mapper::optionsHandled() {
    boost::program_options::options_description desc("Allowed options");

    LanguageDependentAnnotatorFactory::addLanguageDependentOptions(desc);

    desc.add_options()
        ("in-tags", boost::program_options::value<std::string>(), "map edges container all specified tags")
        ("out-tags", boost::program_options::value<std::string>(), "add tags to mapped edges")
        ("consider-text", boost::program_options::value<std::string>(), "")
        ("consider-category", boost::program_options::value<std::string>(), "")
        ("consider-attributes", boost::program_options::value<std::string>(), "")
        ("clone-text", boost::program_options::value<std::string>(), "")
        ("clone-category", boost::program_options::value<std::string>(), "")
        ("add-attributes", boost::program_options::value<std::string>(), "")
        ("set-text", boost::program_options::value<std::string>(), "")
        ("set-category", boost::program_options::value<std::string>(), "")
        ("set-attrs", boost::program_options::value<std::string>(), "")
        ("store", boost::program_options::value<std::string>(), "")
        ("binary-lexicon", boost::program_options::value<std::string>(), "")
        ("plain-text-lexicon", boost::program_options::value<std::string>(), "")
        ("save-binary-lexicon", boost::program_options::value<std::string>(), "")
    ;
    
    return desc;
}

std::list<std::string> Mapper::providedLayerTags() {
    return boost::assign::list_of
        (std::string("mapper"));
}

std::list<std::list<std::string> > Mapper::requiredLayerTags() {
    return
        boost::assign::list_of(std::list<std::string>());
}

std::list<std::string> Mapper::tagsToOperateOn() {
    return inTags_;
}

void Mapper::processEdge(Lattice& lattice, Lattice::EdgeDescriptor edge) {
    //std::string edgeText = lattice.getAnnotationText(edge);

    //std::vector<std::string> records = lexiconBase_.getRecords(edgeText);

    //BOOST_FOREACH(std::string& record, records) {
        addEntry_(lattice, edge, "MAPPER");
    //}
}

void Mapper::addEntry_(
    Lattice& lattice, Lattice::EdgeDescriptor edge, const std::string& record) {

    LayerTagCollection tags = lattice.getLayerTagManager().createTagCollectionFromList(
        outTags_);

    Lattice::EdgeSequence::Builder builder(lattice);
    builder.addEdge(edge);

    lattice.addEdge(
        lattice.getEdgeSource(edge),
        lattice.getEdgeTarget(edge),
        parseRecord_(record),
        tags,
        builder.build());
}

AnnotationItem Mapper::parseRecord_(const std::string& record) {
    size_t separatorPos = record.find_first_of(LEMMA_CATEGORY_SEPARATOR);

    std::string category =
        (separatorPos == std::string::npos
         ? record
         : record.substr(separatorPos + 1));

    return AnnotationItem(category, StringFrag(record));
}

void Mapper::readPlainText_(const boost::filesystem::path& plainTextLexicon) {
    lexiconBase_.readPlainText(plainTextLexicon);
}

void Mapper::saveBinary_(const boost::filesystem::path& binaryLexiconPath) {
    lexiconBase_.saveBinary(binaryLexiconPath);
}

void Mapper::loadBinary_(const boost::filesystem::path& binaryLexiconPath) {
    lexiconBase_.loadBinary(binaryLexiconPath);
}

void Mapper::createTags_(const std::string& trg_lang) {
    inTags_ = providedLayerTags();
    inTags_.push_back(LayerTagManager::getLanguageTag(trg_lang));
}

const std::string Mapper::DEFAULT_BINARY_LEXICON_SPEC
= "%ITSDATA%/%LANG%%TRGLANG%.bin";
