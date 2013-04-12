#include "dictionary.hpp"
#include "logging.hpp"

#include <boost/lexical_cast.hpp>

Dictionary::Dictionary()
{ }

void Dictionary::readDictionary(const std::string& filename)
{
    INFO("Reading text dictionary data");
    std::ifstream in;
    in.open(filename.c_str());
    fsa::FSAMultiStore::Builder builder;
    builder.createFromSeparatedLines(LineIterator(in), LineIterator());
    store_.reset(builder.build());
    in.close();
    INFO("Done");
}

void Dictionary::load(std::string file_name) {
    std::ifstream in;
    in.open(file_name.c_str(), std::ios::in|std::ios::binary);
    load(in);
    in.close();
}

void Dictionary::load(std::ifstream &in) {
    INFO("Loading dictionary data");
    store_.reset(new fsa::FSAMultiStore());
    store_->load(in);
    INFO("Done");
}

void Dictionary::save(std::string file_name) {
    std::ofstream out(file_name.c_str(), std::ios::out|std::ios::binary);
    save(out);
    out.close();
}

void Dictionary::save(std::ofstream &out) {
    INFO("Saving dictionary data");
    store_->save(out);
    INFO("Done");
}

LemmaMap Dictionary::get(const std::string& token)
{
    LemmaMap lemmaMap;
    BOOST_FOREACH(std::string result, store_->get(token))
        parseResult(token, result, lemmaMap);
    return lemmaMap;
}
