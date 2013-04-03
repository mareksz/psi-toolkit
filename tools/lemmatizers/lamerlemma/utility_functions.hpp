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
    
typedef std::map<std::string, std::string> FeatureMap;
typedef std::vector<FeatureMap> FeatureMapSet;
typedef std::pair<FeatureMap, FeatureMapSet> FeatureMapSetPair;
typedef std::map<std::string, FeatureMapSetPair> TagMap;
typedef std::map<std::string, TagMap> LemmaMap;

std::string KLEndEncode(std::string asRef, std::string toCode);

std::string KLEndDecode(std::string asRef, std::string asCode);

std::string parseInput(const std::string& result);

void parseResult(const std::string& key, const std::string& result, LemmaMap &lemmaMapq);

class Line {   
  public:
    friend std::istream &operator>>(std::istream &is, Line &line) {
        std::getline(is, line.data_);
        line.linesRead_++;
        if(line.linesRead_ % 1000000 == 0)
            INFO("[" << line.linesRead_ << + "]");
            
        if(line.data_.size()) {
            std::string parsedData = parseInput(line.data_);
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



#endif