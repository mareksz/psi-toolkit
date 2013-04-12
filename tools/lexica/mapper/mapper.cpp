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

    if (options.count("in-tags") > 0 && options.count("out-tags") > 0) {
        std::string inTagsString = options["in-tags"].as<std::string>();
        std::string outTagsString = options["out-tags"].as<std::string>();

        inTags_ = LayerTagManager::splitCollectionSpecification(inTagsString);
        outTags_ = LayerTagManager::splitCollectionSpecification(outTagsString);
    }
    else {
        throw Exception("missing --in-tags or --out-tags");
    }

    considerText_ = options.count("consider-text") > 0;
    considerCategory_ = options.count("consider-category") > 0;
    considerAttributes_ = options.count("consider-attributes") > 0;

    if (!(considerText_ || considerCategory_ || considerAttributes_))
        throw Exception("you have to specify at least one of the following options: "
                        "--consider-text, --consider-category, --consider-attributes");

    cloneText_ = options.count("clone-text") > 0;
    cloneCategory_ = options.count("clone-category") > 0;
    cloneAttributes_ = options.count("clone-attributes") > 0;

    addAttributes_ = options.count("add-attributes") > 0;

    if (options.count("set-text") > 0)
        setText_ = options["set-text"].as<std::string>();

    if (options.count("set-category") > 0)
        setCategory_ = options["set-category"].as<std::string>();

    if (options.count("set-attributes") > 0)
        setAttributes_ = options["set-attributes"].as<std::string>();

    unknownCloneText_ = options.count("unknown-clone-text") > 0;
    unknownCloneCategory_ = options.count("unknown-clone-category") > 0;
    unknownCloneAttributes_ = options.count("unknown-clone-attributes") > 0;

    if (options.count("unknown-set-text") > 0)
        unknownSetText_ = options["unknown-set-text"].as<std::string>();

    if (options.count("unknown-set-category") > 0)
        unknownSetCategory_ = options["unknown-set-category"].as<std::string>();

    if (options.count("unknown-set-attributes") > 0)
        unknownSetAttributes_ = options["unknown-set-attributes"].as<std::string>();

    if (cloneText_ and setText_)
        throw(Exception("use either --clone-text or --set-text, not both"));

    if (cloneCategory_ and setCategory_)
        throw(Exception("use either --clone-category or --set-category, not both"));

    if (cloneAttributes_ and setAttributes_)
        throw(Exception("use either --clone-attributes or --set-attributes, not both"));

    if (unknownCloneText_ and unknownSetText_)
        throw(Exception("use either --unknown-clone-text or --unknown-set-text, not both"));

    if (unknownCloneCategory_ and unknownSetCategory_)
        throw(Exception("use either --unknown-clone-category or --unknown-set-category, not both"));

    if (unknownCloneAttributes_ and unknownSetAttributes_)
        throw(Exception("use either --unknown-clone-attributes or --unknown-set-attributes, not both"));

    if (setAttributes_) {
        std::stringstream dummyRecord;
        dummyRecord << "dummyLemma" << LEXICON_FIELD_SEPARATOR << "dummyCategory"
            << LEXICON_FIELD_SEPARATOR << setAttributes_.get();

        boost::optional<std::string> dummyText;
        boost::optional<std::string> dummyCategory;
        parseEntry_(dummyRecord.str(), dummyText, dummyCategory, setAttributesParsed_);
    }

    if (unknownSetAttributes_) {
        std::stringstream dummyRecord;
        dummyRecord << "dummyLemma" << LEXICON_FIELD_SEPARATOR << "dummyCategory"
            << LEXICON_FIELD_SEPARATOR << unknownSetAttributes_.get();

        boost::optional<std::string> dummyText;
        boost::optional<std::string> dummyCategory;
        parseEntry_(dummyRecord.str(), dummyText, dummyCategory, unknownSetAttributesParsed_);
    }

    if (options.count("plain-text-lexicon") == 0
       && options.count("binary-lexicon") == 0) {
        throw Exception("no mapper lexicon file given");
    }
    else {
        if (options.count("plain-text-lexicon") > 0) {
            if (options.count("binary-lexicon") > 0)
                throw new Exception(
                    "either --plain-text-lexicon or --binary-lexicon expected, not both");

            boost::filesystem::path plainTextLexiconPath =
                fileFetcher.getOneFile(
                    options["plain-text-lexicon"].as<std::string>());

            readPlainText_(plainTextLexiconPath);
        } else if (options.count("binary-lexicon") > 0) {
            boost::filesystem::path binaryLexiconPath =
                fileFetcher.getOneFile(
                    options["binary-lexicon"].as<std::string>());

            loadBinary_(binaryLexiconPath);
        }

        if (options.count("save-binary-lexicon") > 0) {
            if (lexiconBase_.isEmpty())
                throw new Exception("no data to save");

            boost::filesystem::path binaryLexiconPath(
                options["save-binary-lexicon"].as<std::string>());

            saveBinary_(binaryLexiconPath);
        }
    }
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

    if (!options.count("lang"))
        return std::list<std::string>();

    return boost::assign::list_of(options["lang"].as<std::string>());
}

std::list<std::string> Mapper::providedLayerTags() {
    return std::list<std::string>();
}

std::list<std::list<std::string> > Mapper::requiredLayerTags() {
    return
        boost::assign::list_of(std::list<std::string>());
}

std::list<std::string> Mapper::providedLayerTags(
    const boost::program_options::variables_map& options) {

    std::list<std::string> outTags;

    if (options.count("out-tags") > 0) {
        std::string outTagsString = options["out-tags"].as<std::string>();
        std::list<std::string> outTags
            = LayerTagManager::splitCollectionSpecification(outTagsString);
    }
    else {
        throw Exception("missing --out-tags");
    }

    outTags.push_back("mapper");
    return outTags;
}

std::list<std::list<std::string> > Mapper::requiredLayerTags(
    const boost::program_options::variables_map& options) {

    if (options.count("in-tags") > 0) {
        std::string inTagsString = options["in-tags"].as<std::string>();
        std::list<std::string> inTags
            = LayerTagManager::splitCollectionSpecification(inTagsString);

        return
            boost::assign::list_of(inTags);
    }
    else {
        throw Exception("missing --in-tags");
    }

    return
        boost::assign::list_of(std::list<std::string>());
}

boost::program_options::options_description Mapper::optionsHandled() {
    boost::program_options::options_description desc("Allowed options");

    LanguageDependentAnnotatorFactory::addLanguageDependentOptions(desc);

    desc.add_options()
        ("in-tags", boost::program_options::value<std::string>(),
         "map edges container all specified tags")
        ("out-tags", boost::program_options::value<std::string>(),
         "add tags to mapped edges")
        ("consider-text", "consider text for key creation")
        ("consider-category",
         "consider category for key creation")
        ("consider-attributes", "consider attributes (sorted) for key creation")
        ("clone-text", "clone original text to mapped edge")
        ("clone-category", "clone original category to mapped edge")
        ("clone-attributes", "clone original attributes to mapped edge")
        ("unknown-clone-text", "clone original text to mapped edge for unknown keys")
        ("unknown-clone-category", "clone original category to mapped edge for unknown keys")
        ("unknown-clone-attributes", "clone original attributes to mapped edge for unknown keys")
        ("set-text", boost::program_options::value<std::string>(), "set mapped edge text")
        ("set-category", boost::program_options::value<std::string>(), "set mapped edge category")
        ("set-attributes", boost::program_options::value<std::string>(), "set mapped edge attributes")
        ("unknown-set-text", boost::program_options::value<std::string>(), "set mapped edge text for unknown keys")
        ("unknown-set-category", boost::program_options::value<std::string>(), "set mapped edge category for unknown keys")
        ("unknown-set-attributes", boost::program_options::value<std::string>(), "set mapped edge attributes for unknown keys")
        ("add-attributes", "add attributes from lexicon to mapped edge")
        //("store", boost::program_options::value<std::string>(), "") @TODO
        ("binary-lexicon", boost::program_options::value<std::string>(),
         "path to the lexicon in the binary format")
        ("plain-text-lexicon", boost::program_options::value<std::string>(),
         "path to the lexicon in the plain text format")
        ("save-binary-lexicon", boost::program_options::value<std::string>(),
         "as a side effect the lexicon in the binary format is generated")
    ;

    return desc;
}

std::list<std::string> Mapper::tagsToOperateOn() {
    return inTags_;
}

void Mapper::processEdge(Lattice& lattice, Lattice::EdgeDescriptor edge) {

    std::vector<std::string> keys;
    if (considerText_)
        keys.push_back(lattice.getAnnotationText(edge));
    if (considerCategory_)
        keys.push_back(lattice.getAnnotationCategory(edge));
    if (considerAttributes_) {
        AnnotationItemManager& manager = lattice.getAnnotationItemManager();
        const AnnotationItem item = lattice.getEdgeAnnotationItem(edge);
        typedef std::pair<std::string, std::string> StringPair;
        std::vector<std::string> attributes;
        BOOST_FOREACH(StringPair kv, manager.getValues(item))
            attributes.push_back(kv.first + "=" + kv.second);
        std::sort(attributes.begin(), attributes.end());
        keys.insert(keys.end(), attributes.begin(), attributes.end());
    }

    std::string key = boost::algorithm::join(keys, LEXICON_KEY_FIELD_SEPARATOR);
    std::vector<std::string> records = lexiconBase_.getRecords(key);

    if (!records.empty()) {
        BOOST_FOREACH(std::string& record, records)
            addEntry_(lattice, edge, record);
    }
    else if (unknownCloneText_ || unknownCloneCategory_ || unknownCloneAttributes_
        || unknownSetText_ || unknownSetCategory_ || unknownSetAttributes_) {
        addEntryUnknown_(lattice, edge);
    }
}

void Mapper::addEntry_(
    Lattice& lattice, Lattice::EdgeDescriptor edge, const std::string& record) {

    boost::optional<std::string> text;
    boost::optional<std::string> category;

    typedef std::pair<std::string, std::string> StringPair;
    std::vector<StringPair> attributes;

    parseEntry_(record, text, category, attributes);

    if (cloneText_)
        text = lattice.getAnnotationText(edge);
    if (cloneCategory_)
        category = lattice.getAnnotationCategory(edge);
    if (setText_)
        text = setText_;
    if (setCategory_)
        category = setCategory_;

    if (!text)
        text = record;
    if (!category)
        category = record;

    AnnotationItem ai(category.get(), StringFrag(text.get()));

    if (cloneAttributes_) {
        AnnotationItemManager& manager = lattice.getAnnotationItemManager();
        const AnnotationItem item = lattice.getEdgeAnnotationItem(edge);
        BOOST_FOREACH(StringPair kv, manager.getValues(item))
            manager.setValue(ai, kv.first, kv.second);
    }
    if (setAttributes_) {
        AnnotationItemManager& manager = lattice.getAnnotationItemManager();
        BOOST_FOREACH(StringPair kv, setAttributesParsed_)
            manager.setValue(ai, kv.first, kv.second);
    }
    if (addAttributes_) {
        AnnotationItemManager& manager = lattice.getAnnotationItemManager();
        BOOST_FOREACH(StringPair kv, attributes)
            manager.setValue(ai, kv.first, kv.second);
    }

    LayerTagCollection tags = lattice.getLayerTagManager().createTagCollectionFromList(outTags_);

    Lattice::EdgeSequence::Builder builder(lattice);
    builder.addEdge(edge);

    lattice.addEdge(
        lattice.getEdgeSource(edge),
        lattice.getEdgeTarget(edge),
        ai,
        tags,
        builder.build());
}

void Mapper::addEntryUnknown_(Lattice& lattice, Lattice::EdgeDescriptor edge) {

    boost::optional<std::string> text;
    boost::optional<std::string> category;

    typedef std::pair<std::string, std::string> StringPair;
    std::vector<StringPair> attributes;

    if (unknownCloneText_)
        text = lattice.getAnnotationText(edge);
    if (unknownCloneCategory_)
        category = lattice.getAnnotationCategory(edge);
    if (unknownSetText_)
        text = unknownSetText_;
    if (unknownSetCategory_)
        category = unknownSetCategory_;

    if (!text)
        text = "unknown";
    if (!category)
        category = "unknown";

    AnnotationItem ai(category.get(), StringFrag(text.get()));

    if (unknownCloneAttributes_) {
        AnnotationItemManager& manager = lattice.getAnnotationItemManager();
        const AnnotationItem item = lattice.getEdgeAnnotationItem(edge);
        BOOST_FOREACH(StringPair kv, manager.getValues(item))
            manager.setValue(ai, kv.first, kv.second);
    }
    if (unknownSetAttributes_) {
        AnnotationItemManager& manager = lattice.getAnnotationItemManager();
        BOOST_FOREACH(StringPair kv, unknownSetAttributesParsed_)
            manager.setValue(ai, kv.first, kv.second);
    }

    LayerTagCollection tags = lattice.getLayerTagManager().createTagCollectionFromList(outTags_);

    Lattice::EdgeSequence::Builder builder(lattice);
    builder.addEdge(edge);

    lattice.addEdge(
        lattice.getEdgeSource(edge),
        lattice.getEdgeTarget(edge),
        ai,
        tags,
        builder.build());
}

void Mapper::parseEntry_(const std::string& record,
                         boost::optional<std::string>& text,
                         boost::optional<std::string>& category,
                         std::vector<std::pair<std::string, std::string> >& attributes) {

    std::vector<std::string> fields;
    boost::split(fields, record, boost::is_any_of(LEXICON_FIELD_SEPARATOR));

    if (fields.size() > 0)
        text = fields[0];
    if (fields.size() > 1) {
        category = fields[1];
    }
    if (fields.size() > 2) {
        std::vector<std::string> subfields;
        boost::split(subfields, fields[2], boost::is_any_of(LEXICON_SUBFIELD_SEPARATOR));

        BOOST_FOREACH(std::string subfield, subfields) {
            size_t sep;
            if ((sep = subfield.find_first_of(LEXICON_KEY_VALUE_SEPARATOR))
               != std::string::npos) {
                attributes.push_back(std::make_pair(
                    subfield.substr(0, sep), subfield.substr(sep + 1)));
            }
            else {
                attributes.push_back(std::make_pair(subfield, "1"));
            }
        }
    }
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
