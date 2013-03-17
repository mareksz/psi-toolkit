#ifndef UTILITY_FUNCTIONS_HDR
#define UTILITY_FUNCTIONS_HDR
 
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <iterator>
#include <boost/lexical_cast.hpp>

#include "psi_exception.hpp"
#include "morfologik_tags_parser.hpp"
#include "logging.hpp"

namespace psi {
    
    typedef std::map<std::string, std::string> FeatureMap;
    typedef std::vector<FeatureMap> FeatureMapSet;
    typedef std::map<std::string, FeatureMapSet> TagMap;
    typedef std::map<std::string, TagMap> LemmaMap;
    
    std::string KLEndEncode(std::string asRef, std::string toCode);
    
    std::string KLEndDecode(std::string asRef, std::string asCode);
    
    std::string parseInput(const std::string& result,
                           const std::string& separator,
                           bool parseTag, bool parseMorpho);
    
    void parseResult(const std::string& result, LemmaMap &lemmaMap,
                     const std::string& separator, bool parseTag, bool parseMorpho);
    
    class Line {   
      public:
        friend std::istream &operator>>(std::istream &is, Line &line) {
            std::getline(is, line.data_);
            line.linesRead_++;
            if(line.linesRead_ % 1000000 == 0)
                INFO("[" << line.linesRead_ << + "]");
                
            if(line.data_.size()) {
                std::string parsedData = parseInput(line.data_, "\t", true, true);
                line.data_.swap(parsedData);
            }
            return is;
        }
        
        operator std::string() const {
            return data_;
        }
        
      private:
        std::string data_;
        static size_t linesRead_;
    };
    
    typedef std::istream_iterator<Line> LineIterator;
    
}

#endif