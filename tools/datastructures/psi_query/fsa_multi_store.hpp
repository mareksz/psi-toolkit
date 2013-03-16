#ifndef FSA_MULTI_STORE_HDR
#define FSA_MULTI_STORE_HDR

#include <iterator>
#include <string>

#include "fsa_bindfsa.hpp"
#include "fsa_ndfsa.hpp"
#include "fsa_algorithms.hpp"

namespace psi {
  namespace fsa {
    
    class FSAMultiStore
    {
      public:
        FSAMultiStore(const std::string& separator = "\t");
        
        template <typename OutputIterator>
        void get(const std::string& key, OutputIterator iterator) const {
            StringFSA queryFSA(key);
            concatenate(queryFSA, searchSuffixFSA_);
            intersect(queryFSA, storeFSA_, true);
            language(queryFSA, iterator);
        }
        
        std::vector<std::string> get(const std::string& key) const;
        
        void load(const std::string& filename);
        void save(const std::string& filename);
        
        class Builder {
          public:
            Builder(const std::string& separator = "\t");
            ~Builder();
      
            void add(const std::string& key, const std::string& value);
            void addSeparatedLine(const std::string& line);
            
            template <typename InputIterator>
            void createFromSeparatedLines(InputIterator it, InputIterator end)
            {
                std::vector<std::string> lines(it, end);
                std::sort(lines.begin(), lines.end());
                createFromSeparatedSortedLines(lines.begin(), lines.end());
            }
            
            template <typename InputIterator>
            void createFromSeparatedSortedLines(InputIterator it, InputIterator end) {
                StringBinFSA localStore; 
                minimize_incremental(localStore, it, end);
                store_->storeFSA_.swap(localStore); // erase previous data
            }
            
            FSAMultiStore* build();
            
          private:
            FSAMultiStore* store_;  
            std::string separator_;
            
            bool singleLineAdded_;
        };

        
      private:
        StringBinFSA storeFSA_;
        StringFSA searchSuffixFSA_;
        std::string separator_;
        
        void buildSearchSuffix();
    };
  }
  
}
#endif
