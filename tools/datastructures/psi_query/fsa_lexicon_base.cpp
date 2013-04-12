#include "config.hpp"
#include "fsa_lexicon_base.hpp"
#include "logging.hpp"

#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/range/sub_range.hpp>

namespace fsa {

void FSALexiconBase::readPlainText(const boost::filesystem::path& plainTextLexicon) {

    FSAMultiStore::Builder storeBuilder;

    boost::filesystem::ifstream plainTextStream(plainTextLexicon);

    std::string line;
    typedef boost::sub_range<std::vector<std::string> > SubRange;

    while (std::getline(plainTextStream, line)) {
        removeComment_(line);

        if (isEmptyLine_(line))
            continue;

        std::vector<std::string> fields;
        boost::split(fields, line, boost::is_any_of(LEXICON_TEXT_FIELD_SEPARATORS));
        fields.erase(std::remove_if (
                         fields.begin(), fields.end(),
                         boost::bind( &std::string::empty, _1 )), fields.end());

        BOOST_FOREACH(std::string& field, fields) {
            field = quoter_.unescape(field);
        }

        if (fields.size() < 2)
            throw Exception(
                std::string("at least two fields expected in plain text lexicon, was: `")
                + line + "`");

        std::string key = fields[0];
        std::string value = boost::join(SubRange(fields.begin() + 1, fields.end()),
                                        LEXICON_FIELD_SEPARATOR);
        storeBuilder.add(key, value);
    }

    store_.reset(storeBuilder.build());
}

void FSALexiconBase::saveBinary(const boost::filesystem::path& binaryLexiconPath) {
    WARN("saving lexicon to `" << binaryLexiconPath.string() << "`");
    store_->save(binaryLexiconPath.string());
    WARN("lexicon saved");
}

void FSALexiconBase::loadBinary(const boost::filesystem::path& binaryLexiconPath) {
    store_.reset(new FSAMultiStore());
    store_->load(binaryLexiconPath.string());
}

bool FSALexiconBase::isEmpty() {
    return !store_;
}

std::vector<std::string> FSALexiconBase::getRecords(const std::string& text) {
    return store_->get(text);
}

void FSALexiconBase::removeComment_(std::string& s) {
    size_t hashPos = s.find_first_of('#');

    if (hashPos != std::string::npos)
        s = s.substr(0, hashPos);
}

bool FSALexiconBase::isEmptyLine_(const std::string& s) {
    return s.find_first_not_of(LEXICON_TEXT_FIELD_SEPARATORS);
}

}
