#include "dictionary.hpp"
#include "logging.hpp"

#include <boost/lexical_cast.hpp>

namespace psi {

Dictionary::Dictionary(bool hasPos, bool hasMorpho)
 : hasPos_(hasPos||hasMorpho), hasMorpho_(hasMorpho), store_(0)
{ }

Dictionary::~Dictionary() {
    if(store_)
        delete store_;
}

void Dictionary::readDictionary(const std::string& filename)
{
    INFO("Reading text dictionary data");
    std::ifstream in;
    in.open(filename.c_str());
    psi::fsa::FSAMultiStore::Builder builder("\t");
    builder.createFromSeparatedLines(LineIterator(in), LineIterator());
    store_ = builder.build();
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
    if(store_)
        delete store_;
    store_ = new fsa::FSAMultiStore();
    store_->load(in);
    in.read((char*) &hasPos_, sizeof(bool));
    in.read((char*) &hasMorpho_, sizeof(bool));
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
    out.write((char*) &hasPos_, sizeof(bool));
    out.write((char*) &hasMorpho_, sizeof(bool));
    INFO("Done");
}
    
LemmaMap Dictionary::get(const std::string& token)
{
    LemmaMap lemmaMap;
    BOOST_FOREACH(std::string result, store_->get(token))
        parseResult(result, lemmaMap, "\t", hasPos_, hasMorpho_);
    return lemmaMap;
}

}