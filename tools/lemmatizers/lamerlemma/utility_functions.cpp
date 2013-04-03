#include "config.hpp"
#include "utility_functions.hpp"

#include <iostream>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
    
size_t Line::linesRead_ = 0;

std::string parseInput(const std::string& input)
{
    std::vector<std::string> fields;
    boost::split(fields, input, boost::is_any_of(LEXICON_TEXT_FIELD_SEPARATORS));
    
    std::string form, lemma, pos;
    if(fields.size() < 1)
        throw PsiException("Missing form: " + input);
    form = fields[0];
    
    if(fields.size() < 2)
        throw PsiException("Missing lemma code " + input);
    lemma = fields[1];
    
    std::stringstream output;
    output << form << LEXICON_FIELD_SEPARATOR;
    
    std::string code = KLEndEncode(form, lemma);
    output << code; 
    
    if(fields.size() >= 3) {
        pos = fields[2];
        output << LEXICON_FIELD_SEPARATOR << pos;
        
        if(fields.size() >= 4)
            output << LEXICON_FIELD_SEPARATOR << fields[3];
    }
    return output.str();
}
    
void parseResult(const std::string& key, const std::string& result, LemmaMap &lemmaMap) {
    
    std::vector<std::string> fields;
    boost::split(fields, result, boost::is_any_of(LEXICON_FIELD_SEPARATOR));
    
    std::string form, code, tag, morpho;
    form = key;
    
    if(fields.size() < 1)
        throw PsiException("Missing lemma code: " + result);
    code = fields[0];
    
    std::string lemma = KLEndDecode(form, code);
            
    if(!lemmaMap.count(lemma))
        lemmaMap[lemma] = TagMap();
    
    if(fields.size() >= 2) {
        std::string tag = fields[1];
        
        if(!lemmaMap[lemma].count(tag))
            lemmaMap[lemma][tag] = FeatureMapSetPair();
        
        std::vector<std::string> posattrfields;
        boost::split(posattrfields, tag, boost::is_any_of(LEXICON_SUBFIELD_SEPARATOR));
        BOOST_FOREACH(std::string posattr, posattrfields) {
            size_t pos;
            if((pos = posattr.find(LEXICON_KEY_VALUE_SEPARATOR)) != std::string::npos) {
                std::string key = posattr.substr(0, pos);
                std::string value = posattr.substr(pos + 1);
                lemmaMap[lemma][tag].first[key] = value;    
            }
            else
                lemmaMap[lemma][tag].first["pos"] = posattr;
        }
        
        lemmaMap[lemma][tag].second.push_back(FeatureMap());
        
        if(fields.size() == 3) {
            std::vector<std::string> morphofields;
            boost::split(morphofields, fields[2], boost::is_any_of(LEXICON_SUBFIELD_SEPARATOR));
            BOOST_FOREACH(std::string morpho, morphofields) {
                size_t pos;
                if((pos = morpho.find(LEXICON_KEY_VALUE_SEPARATOR)) != std::string::npos) {
                    std::string key = morpho.substr(0, pos);
                    std::string value = morpho.substr(pos + 1);
                    lemmaMap[lemma][tag].second.back()[key] = value;    
                }
                else
                    lemmaMap[lemma][tag].second.back()[morpho] = "1";
            }
        }
    }
}

std::string KLEndEncode(std::string asRef, std::string toCode)
{
    size_t asRefLen = asRef.size();
    size_t toCodeLen = toCode.size();
            
    size_t maxLength = 0;
    size_t maxi = 0;
    for(size_t i = 0; i < asRefLen; i++) {
        if(asRef[i] == toCode[0]) {
            size_t length = 1;
            for(size_t j = 1; j < toCodeLen && i+j < asRefLen
                && asRef[i+j] == toCode[j]; j++)
                length++;
                    
            if(length > maxLength) {
                maxLength = length;
                maxi = i;
            }
        }
        if(asRefLen - i < maxLength)
            break;
    }
    
    char K = 65 + maxi;
    char L = 65 + asRefLen - (maxi + maxLength);
    std::string end = toCode.substr(maxLength); 
    
    std::stringstream ss;
    ss << K << L << end;
    return ss.str();
}

std::string KLEndDecode(std::string asRef, std::string asCode)
{
    char K = asCode[0];
    char L = asCode[1];
    std::string end = asCode.substr(2);
    
    size_t asRefLen = asRef.size();
    
    int cutFront = K - 65;
    int cutBack  = L - 65;
    
    size_t decodedLen = asRefLen-cutFront-cutBack;
    
    std::stringstream decoded;
    decoded << asRef.substr(cutFront, decodedLen) << end;
    return decoded.str();
}
