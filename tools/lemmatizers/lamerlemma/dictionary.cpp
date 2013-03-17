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
    in.exceptions(std::ifstream::failbit|std::ifstream::badbit);
    try {
        in.open(filename.c_str());
        psi::fsa::FSAMultiStore::Builder builder("\t");
        builder.createFromSeparatedLines(LineIterator(in), LineIterator());
        store_ = builder.build();
    }
    catch(std::ifstream::failure e) {
        WARN("An error occured when trying to read the lemmatizer text file: "
             << e.what());
    }
    in.close();
    INFO("Done");
}

void Dictionary::load(std::string file_name) {
    std::ifstream in;
    in.exceptions(std::ifstream::failbit|std::ifstream::badbit);
    try {
        in.open(file_name.c_str(), std::ios::in|std::ios::binary);
        load(in);
    }
    catch(std::exception e) {
        WARN("An error occured when trying to load the lemmatizer file: "
             << e.what());
    }
    in.close();
}

void Dictionary::load(std::ifstream &in) {
    INFO("Loading dictionary data");
    if(store_)
        delete store_;
    store_ = new fsa::FSAMultiStore();
    store_->load(in);
    char buffer[sizeof("Additional metadata")];
    DEBUG(in.tellg());
    in.read(buffer, sizeof("Additional metadata"));
    DEBUG(in.tellg());
    in.read((char*) &hasPos_, sizeof(bool));
    DEBUG(in.tellg());
    in.read((char*) &hasMorpho_, sizeof(bool));
    DEBUG(in.tellg());
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
    DEBUG(out.tellp());
    out.write("Additional metadata", sizeof("Additional metadata"));
    DEBUG(out.tellp());
    out.write((char*) &hasPos_, sizeof(bool));
    DEBUG(out.tellp());
    out.write((char*) &hasMorpho_, sizeof(bool));
    DEBUG(out.tellp());
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