#ifndef FSA_MULTI_STORE_HDR
#define FSA_MULTI_STORE_HDR

#include <iterator>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/function_output_iterator.hpp>

#include "logging.hpp"
#include "config.hpp"

#include "fsa_bindfsa.hpp"
#include "fsa_ndfsa.hpp"
#include "fsa_algorithms.hpp"

namespace fsa {

class FSAMultiStore
{
  private:

    template <class OutputIterator>
    class KeyTruncator {
      public:
        KeyTruncator(const std::string& key, OutputIterator& iterator)
          : key_(key + LEXICON_FIELD_SEPARATOR), iterator_(iterator) { }

        void operator()(const std::string& record) {
            if (key_ == record.substr(0, key_.size()))
                *iterator_ = record.substr(key_.size());
            else
                *iterator_ = record;
            iterator_++;
        }

      private:
        const std::string key_;
        OutputIterator& iterator_;
    };

  public:
    FSAMultiStore();

    template <typename OutputIterator>
    void get(const std::string& key, OutputIterator iterator) const {
        StringFSA queryFSA(key);
        concatenate(queryFSA, searchSuffixFSA_);
        intersect(queryFSA, storeFSA_, true);

        typedef KeyTruncator<OutputIterator> Trunc;
        typedef boost::function_output_iterator<Trunc> TruncIt;

        language<StringFSA, TruncIt, std::string>(queryFSA, TruncIt(Trunc(key, iterator)));
    }

    std::vector<std::string> get(const std::string& key) const;

    void load(const std::string& filename);
    void load(std::istream&);

    void save(const std::string& filename);
    void save(std::ostream&);

    class Builder {
      public:
        Builder();
        ~Builder();

        void add(const std::string& key, const std::string& value);
        void addSeparatedLine(const std::string& line);

        template <typename InputIterator>
        void createFromSeparatedLines(InputIterator it, InputIterator end)
        {
            std::vector<std::string> lines(it, end);
            INFO("Sorting " << lines.size() << " input lines");
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
    };


  private:

    StringBinFSA storeFSA_;
    StringFSA searchSuffixFSA_;

    void buildSearchSuffix();
};

}


#endif
