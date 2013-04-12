#include "config.hpp"
#include "fsa_multi_store.hpp"

namespace fsa {

FSAMultiStore::Builder::Builder()
  : store_(new FSAMultiStore()) { }

FSAMultiStore::Builder::~Builder()
{
    delete store_;
}

void FSAMultiStore::Builder::add(const std::string& key, const std::string& value)
{
    addSeparatedLine(key + LEXICON_FIELD_SEPARATOR + value);
}

void FSAMultiStore::Builder::addSeparatedLine(const std::string& line)
{
    //@todo currently not implemented
    std::cerr << line << std::endl;
    abort();
}

FSAMultiStore* FSAMultiStore::Builder::build() {
    FSAMultiStore* ret = store_;
    store_ = 0;
    return ret;
}

FSAMultiStore::FSAMultiStore()
{
    buildSearchSuffix();
}

std::vector<std::string> FSAMultiStore::get(const std::string& key) const
{
    std::vector<std::string> words;
    get(key, std::back_inserter(words));
    return words;
}

void FSAMultiStore::load(const std::string& filename)
{
    std::ifstream in(filename.c_str(), std::ios::in|std::ios::binary);
    load(in);
    in.close();
}

void FSAMultiStore::load(std::istream &in)
{
    storeFSA_.load(in);
}

void FSAMultiStore::save(const std::string& filename)
{
    std::ofstream out(filename.c_str(), std::ios::out|std::ios::binary);
    save(out);
    out.close();
}

void FSAMultiStore::save(std::ostream &out)
{
    storeFSA_.save(out);
}

void FSAMultiStore::buildSearchSuffix()
{
    StringFSA separatorFSA(LEXICON_FIELD_SEPARATOR);
    concatenate(searchSuffixFSA_, separatorFSA);
    StringFSA anyFSA(ANY);
    kleene_plus(anyFSA);
    concatenate(searchSuffixFSA_, anyFSA);
}

}
