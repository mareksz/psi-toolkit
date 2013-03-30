#include "config.hpp"
#include "lexicon_base.hpp"
#include "logging.hpp"

#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/range/sub_range.hpp> 

void LexiconBase::readPlainText(const boost::filesystem::path& plainTextLexicon) {

    KeyValueStore::Builder storeBuilder;

    boost::filesystem::ifstream plainTextStream(plainTextLexicon);

    std::string line;
    std::string prevKey;
    std::string stringSoFar;
    bool firstEntry = true;

    typedef boost::sub_range<std::vector<std::string> > SubRange; 
    
    while (std::getline(plainTextStream, line)) {
        removeComment_(line);

        if (isEmptyLine_(line))
            continue;

        std::vector<std::string> fields;
        boost::split(fields, line, boost::is_any_of(LEXICON_TEXT_FIELD_SEPARATORS));
        fields.erase(std::remove_if(
                         fields.begin(), fields.end(),
                         boost::bind( &std::string::empty, _1 )), fields.end());

        BOOST_FOREACH(std::string& field, fields) {
            field = quoter_.unescape(field);
        }

        if (fields.size() < 2)
            throw Exception(
                std::string("at least two fields expected in plain text lexicon, was: `")
                + line + "`");

        if (firstEntry) {
            prevKey = fields[0];
            stringSoFar = boost::join(SubRange(fields.begin() + 1, fields.end()),
                                      LEXICON_FIELD_SEPARATOR);
            firstEntry = false;
        } else {
            if (fields[0] == prevKey) {
                stringSoFar += LEXICON_RECORD_SEPARATOR;
                stringSoFar += boost::join(SubRange(fields.begin() + 1, fields.end()),
                                           LEXICON_FIELD_SEPARATOR);
            }
            else {
                storeBuilder.add(prevKey, stringSoFar);
                prevKey = fields[0];
                stringSoFar = boost::join(SubRange(fields.begin() + 1, fields.end()),
                                          LEXICON_FIELD_SEPARATOR);
            }
        }
    }

    if (!firstEntry)
        storeBuilder.add(prevKey, stringSoFar);

    store_.reset(storeBuilder.build());
}

void LexiconBase::saveBinary(const boost::filesystem::path& binaryLexiconPath) {
    WARN("saving lexicon to `" << binaryLexiconPath.string() << "`");

    store_->save(binaryLexiconPath.string());

    WARN("lexicon saved");
}

void LexiconBase::loadBinary(const boost::filesystem::path& binaryLexiconPath) {
    store_.reset(new KeyValueStore());
    store_->load(binaryLexiconPath.string());
}

bool LexiconBase::isEmpty() {
    return !store_;
}

std::vector<std::string> LexiconBase::getRecords(const std::string& text) {
    boost::optional<std::string> entryFound = store_->get(text);
    std::vector<std::string> records;

    if (entryFound) {
        boost::split(records, entryFound.get(), boost::is_any_of(LEXICON_RECORD_SEPARATOR));
    }

    return records;
}

void LexiconBase::removeComment_(std::string& s) {
    size_t hashPos = s.find_first_of('#');

    if (hashPos != std::string::npos)
        s = s.substr(0, hashPos);
}

bool LexiconBase::isEmptyLine_(const std::string& s) {
    return s.find_first_not_of(LEXICON_TEXT_FIELD_SEPARATORS);
}
