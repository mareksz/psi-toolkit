#ifndef BRACKET_PRINTER_HDR
#define BRACKET_PRINTER_HDR


#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>

#include "edge_data.hpp"
#include "edge_print_data.hpp"


class BracketPrinter {

public:

    BracketPrinter(
        std::vector<std::string> patterns,
        std::string tagSeparator,
        std::string avPairsSeparator,
        std::string avSeparator
    ) :
        patterns_(patterns),
        tagSeparator_(tagSeparator),
        avPairsSeparator_(avPairsSeparator),
        avSeparator_(avSeparator)
    { }

    std::set<EdgePrintData> print(
        std::set<EdgeData> edgeDataSet
    ) {
        return print_< std::set<EdgeData>, std::set<EdgePrintData> >(edgeDataSet);
    }

    std::vector<EdgePrintData> print(
        std::vector<EdgeData> edgeDataVector
    ) {
        return print_< std::vector<EdgeData>, std::vector<EdgePrintData> >(edgeDataVector);
    }

private:

    template <typename InputContainer, typename OutputContainer>
    OutputContainer print_(InputContainer edgeDataContainer);

    std::vector<std::string> patterns_;
    std::string tagSeparator_;
    std::string avPairsSeparator_;
    std::string avSeparator_;

    char close_(char c) {
        switch (c) {
            case '(': return ')';
            case ')': return '(';
            case '[': return ']';
            case ']': return '[';
            case '{': return '}';
            case '}': return '{';
            case '<': return '>';
            case '>': return '<';
            default: return c;
        }
    }

    template <typename Element>
    void insertElementIntoContainer_(
        std::set<Element> & aSet,
        Element & anElement
    ) {
        aSet.insert(anElement);
    }

    template <typename Element>
    void insertElementIntoContainer_(
        std::vector<Element> & aVector,
        Element & anElement
    ) {
        aVector.push_back(anElement);
    }

};


template <typename InputContainer, typename OutputContainer>
OutputContainer BracketPrinter::print_(InputContainer edgeDataContainer) {
    OutputContainer result;
    BOOST_FOREACH(EdgeData edgeData, edgeDataContainer) {
        EdgePrintData resultElement(edgeData);
        EdgeData thisEdgeData;
        InputContainer edgeDataSubcontainer;
        std::string separator;
        std::string subpattern;
        BOOST_FOREACH(std::string pattern, patterns_) {
            std::stringstream resSs;
            size_t i = 0;
            while (i < pattern.length()) {
                if (pattern[i] == '%' && i+1 < pattern.length()) {
                    switch (pattern[i+1]) {
                    case 'T':
                        {
                            std::stringstream tagSs;
                            BOOST_FOREACH(std::string tag, edgeData.tags) {
                                if (!tagSs.str().empty()) {
                                    tagSs << tagSeparator_;
                                }
                                tagSs << tag;
                            }
                            resSs << tagSs.str();
                            thisEdgeData.tags = edgeData.tags;
                            i += 2;
                        }
                        break;
                    case 'c':
                        {
                            resSs << edgeData.category;
                            thisEdgeData.category = edgeData.category;
                            i += 2;
                        }
                        break;
                    case 't':
                        {
                            resSs << edgeData.text;
                            thisEdgeData.text = edgeData.text;
                            i += 2;
                        }
                        break;
                    case 'a':
                        {
                            if (i+2 < pattern.length()) {
                                size_t from = i+3;
                                size_t to = pattern.find(close_(pattern[i+2]), from);
                                if (to != std::string::npos) {
                                    std::string attr = pattern.substr(from, to-from);
                                    resSs << edgeData.avMap[attr];
                                    thisEdgeData.avMap[attr] = edgeData.avMap[attr];
                                    i = to + 1;
                                } else {
                                    resSs << "%a";
                                    i += 2;
                                }
                            } else {
                                resSs << "%a";
                                i += 2;
                            }
                        }
                        break;
                    case 'A':
                        {
                            std::stringstream avSs;
                            typedef std::pair<std::string, std::string> StrStrPair;
                            BOOST_FOREACH(StrStrPair avPair, edgeData.avMap) {
                                if (!avSs.str().empty()) {
                                    avSs << avPairsSeparator_;
                                }
                                avSs << avPair.first << avSeparator_ << avPair.second;
                            }
                            resSs << avSs.str();
                            thisEdgeData.avMap = edgeData.avMap;
                            i += 2;
                        }
                        break;
                    case 's':
                        {
                            resSs << edgeData.score;
                            thisEdgeData.score = edgeData.score;
                            i += 2;
                        }
                        break;
                    case '*':
                        {
                            if (i+2 < pattern.length()) {
                                size_t from = i+3;
                                size_t to = pattern.find(close_(pattern[i+2]), from);
                                if (pattern[to+1] == pattern[i+2]) {
                                    size_t from2 = to+2;
                                    size_t to2 = pattern.find(close_(pattern[i+2]), from2);
                                    if (to2 != std::string::npos) {
                                        separator = pattern.substr(from, to-from);
                                        subpattern = pattern.substr(from2, to2-from2);
                                        resSs << "%*****";
                                        i = to2 + 1;
                                    } else {
                                        resSs << "%*";
                                        i += 2;
                                    }
                                } else {
                                    resSs << "%*";
                                    i += 2;
                                }
                            } else {
                                resSs << "%*";
                                i += 2;
                            }
                        }
                        break;
                    default:
                        {
                            resSs << '%';
                            i++;
                        }
                        break;
                    }
                } else {
                    resSs << pattern[i];
                    i++;
                }
            }
            resultElement.addPrintedElement(resSs.str());
        }
        if (!subpattern.empty()) {
            BOOST_FOREACH(EdgeData edgeData2, edgeDataContainer) {
                if (!thisEdgeData.category.empty() && edgeData2.category != thisEdgeData.category) {
                    continue;
                }
                if (!thisEdgeData.text.empty() && edgeData2.text != thisEdgeData.text) {
                    continue;
                }
                if (!thisEdgeData.tags.empty() && edgeData2.tags != thisEdgeData.tags) {
                    continue;
                }
                if (thisEdgeData.score != 0.0 && edgeData2.score != thisEdgeData.score) {
                    continue;
                }
                if (!thisEdgeData.avMap.empty()) {
                    bool shallWeContinue = false;
                    typedef std::pair<std::string, std::string> StrStrPair;
                    BOOST_FOREACH(StrStrPair avPair, thisEdgeData.avMap) {
                        if (edgeData2.avMap[avPair.first] != thisEdgeData.avMap[avPair.first]) {
                            shallWeContinue = true;
                            break;
                        }
                    }
                    if (shallWeContinue) {
                        continue;
                    }
                }
                insertElementIntoContainer_<EdgeData>(edgeDataSubcontainer, edgeData2);
            }
            std::vector<std::string> subpatterns
                = boost::assign::list_of(subpattern);
            BracketPrinter subprinter(
                subpatterns,
                tagSeparator_,
                avPairsSeparator_,
                avSeparator_
            );
            OutputContainer joinProduct
                = subprinter.print(edgeDataSubcontainer);
            std::stringstream joinSs;
            BOOST_FOREACH(EdgePrintData epd, joinProduct) {
                if (!joinSs.str().empty()) {
                    joinSs << separator;
                }
                joinSs << epd.printedElements[0];
            }
            BOOST_FOREACH(std::string & s, resultElement.printedElements) {
                size_t found = s.find("%*****");
                if (found != std::string::npos) {
                    s.replace(found, 6, joinSs.str());
                }
            }
        }
        insertElementIntoContainer_<EdgePrintData>(result, resultElement);
    }
    return result;
}


#endif
