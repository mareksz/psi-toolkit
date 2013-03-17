#include "utility_functions.hpp"

#include <iostream>
#include <boost/foreach.hpp>

namespace psi {
    
    size_t Line::linesRead_ = 0;
    
    std::string parseInput(const std::string& input, const std::string& separator,
                    bool parseTag, bool parseMorpho)
    {
        std::stringstream inputStream(input);
        std::string form, lemma, pos, morpho;
        if(!(inputStream >> form))
            throw new PsiException("Missing form: " + input);
        if(!(inputStream >> lemma))
            throw new PsiException("Missing lemma code " + input);
        
        std::stringstream output;
        output << form << separator;
        
        std::string code = KLEndEncode(form, lemma);
        output << code; 
        
        MorfologikTagsParser tagsParser;
        
        if(parseTag || parseMorpho) {
            if(!(inputStream >> pos))
                throw new PsiException("Missing part-of-speech tag: " + input);
            
            output << separator << pos;
            if(parseMorpho)
                while(inputStream >> morpho)
                    output << separator << morpho;
        }
        return output.str();
    }
        
    void parseResult(const std::string& result, LemmaMap &lemmaMap,
                     const std::string& separator, bool parseTag, bool parseMorpho) {
        std::stringstream resultStream(result);
        std::string form, code, tag, morpho;
        if(!(resultStream >> form))
            throw new PsiException("Missing form: " + result);
        if(!(resultStream >> code))
            throw new PsiException("Missing lemma code: " + result);
        
        std::string lemma = KLEndDecode(form, code);
                
        if(!lemmaMap.count(lemma))
            lemmaMap[lemma] = TagMap();
        
        if(parseTag || parseMorpho) {
            if(!(resultStream >> tag))
                throw new PsiException("Missing tag: " + result);
            
            if(!lemmaMap[lemma].count(tag))
                lemmaMap[lemma][tag] = FeatureMapSet();
            
            lemmaMap[lemma][tag].push_back(FeatureMap());
            
            if(parseMorpho) {
                while(resultStream >> morpho) {
                    int pos;
                    if((pos = morpho.find("=")) != std::string::npos) {
                        std::string key = morpho.substr(0, pos);
                        std::string value = morpho.substr(pos + 1);
                        lemmaMap[lemma][tag].back()[key] = value;    
                    }
                    else
                        lemmaMap[lemma][tag].back()[morpho] = "1";
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
    
}
