#include "fsa_multi_store.hpp"

namespace psi {
  namespace fsa {
    
    FSAMultiStore::Builder::Builder(const std::string& separator)
      : store_(new FSAMultiStore(separator)), separator_(separator),
        singleLineAdded_(false) { }
    
    FSAMultiStore::Builder::~Builder()
    {
        delete store_;
    }

    void FSAMultiStore::Builder::add(const std::string& key, const std::string& value)
    {
        addSeparatedLine(key + separator_ + value);  
    }
    
    void FSAMultiStore::Builder::addSeparatedLine(const std::string& line)
    {
        StringFSA lineFSA(line);
        unify(store_->storeFSA_, lineFSA);
        singleLineAdded_ = true;
    }
    
    FSAMultiStore* FSAMultiStore::Builder::build() {
        if(singleLineAdded_)
            minimize(store_->storeFSA_);
        
        FSAMultiStore* ret = store_;
        store_ = 0;
        return ret;
    }
  
    FSAMultiStore::FSAMultiStore(const std::string& separator) : separator_(separator)
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
        size_t separatorSize;
        in.read((char*)&separatorSize, sizeof(size_t));
        separator_.resize(separatorSize);
        in.read(&separator_[0], separatorSize);
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
        size_t separatorSize = separator_.size();
        out.write((char*)&separatorSize, sizeof(size_t));
        out.write(separator_.c_str(), separatorSize);
        storeFSA_.save(out);
    }
    
    void FSAMultiStore::buildSearchSuffix()
    {
        StringFSA separatorFSA(separator_);
        concatenate(searchSuffixFSA_, separatorFSA);
        StringFSA anyFSA(ANY);
        kleene_plus(anyFSA);
        concatenate(searchSuffixFSA_, anyFSA);
    }
    
  }
}
