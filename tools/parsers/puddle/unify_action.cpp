#include "unify_action.hpp"
#include <iostream>

namespace poleng {

namespace bonsai {
    namespace puddle {

UnifyAction::UnifyAction(std::vector<std::string> aUnifiedPatterns,
        std::vector<std::string> aUnifiedAttributes,
        std::vector<int> aTokenIndices) {
    init(aUnifiedPatterns, aUnifiedAttributes, aTokenIndices);
}

bool UnifyAction::apply(Lattice &lattice, std::string langCode,
        int matchedStartIndex, RuleTokenSizes &ruleTokenSizes,
        std::list<Lattice::EdgeSequence> &rulePartitions) {

    std::vector<std::vector<std::string> > unifiedInterpretations =
        generateInterpretationsVector(lattice, langCode, ruleTokenSizes,
                matchedStartIndex);

    std::set<std::string> unifiedStrings = generateUnifiedInterpretationStrings(
            unifiedInterpretations);

    std::vector<std::list<Lattice::EdgeDescriptor> > edgesToKeep =
        generateUnifiedEdgesList(lattice, langCode, ruleTokenSizes,
                matchedStartIndex, unifiedStrings);

    std::list<Lattice::EdgeSequence> newRulePartitions;
    for (std::vector<std::list<Lattice::EdgeDescriptor> >::iterator listIt =
            edgesToKeep.begin(); listIt != edgesToKeep.end(); ++ listIt) {
        std::list<Lattice::EdgeSequence> tmpPartitions =
            lattice::getPartitionsContainingEdges(lattice, rulePartitions,
                    *listIt);
        newRulePartitions.insert(newRulePartitions.end(),
                tmpPartitions.begin(), tmpPartitions.end());
    }
    if (! newRulePartitions.empty()) {
        discardUnunifiedEdges(lattice, langCode, ruleTokenSizes,
                matchedStartIndex, edgesToKeep);
        rulePartitions = newRulePartitions;
        return true;
    }

    return true;
}

bool UnifyAction::test(Lattice &lattice, std::string langCode,
        int matchedStartIndex, RuleTokenSizes &ruleTokenSizes,
        std::list<Lattice::EdgeSequence>&) {

    bool toApply = isUnifyingPossible(lattice, langCode, matchedStartIndex,
            ruleTokenSizes);
    return toApply;
}

void UnifyAction::init(std::vector<std::string> aUnifiedPatterns,
        std::vector<std::string> aUnifiedAttributes,
        std::vector<int> aTokenIndices) {
    nullAgreement = true; //@todo: to ma byc parametrem parsera

    type = "unify";

    tokenIndices = aTokenIndices;
    unifiedAttributes = aUnifiedAttributes;
    unifiedPatterns = aUnifiedPatterns;
}

std::vector<std::vector<std::string> > UnifyAction::generateInterpretationsVector(
        Lattice &lattice, std::string langCode, RuleTokenSizes &ruleTokenSizes,
        int matchedStartIndex) {
    std::vector<std::vector<std::string> > unifiedInterpretations;

    for (std::vector<int>::iterator index_it = tokenIndices.begin();
            index_it != tokenIndices.end(); ++ index_it) {

        int count, before;
        if (! util::getUnifyActionTokenParams(ruleTokenSizes, *index_it,
                    count, before) )
            continue;

        //Lattice::VertexDescriptor vertex = matchedStartIndex + before;
        Lattice::VertexDescriptor vertex = lattice::getVertex(lattice,
                langCode, before, matchedStartIndex);
//        //@todo: czy to sprawdzenie jest nadal konieczne?
//        ta funkcja getVertex nie robi czegos takiego?
//        while (lattice::getTopEdges(lattice, vertex).size() == 0) {
//            before ++;
//            vertex = matchedStartIndex + before;
//        }

        int offset = vertex;
        int vertexI = 0;
        while (vertexI < count) {
            vertex = lattice::getVertex(lattice, langCode, vertexI, offset);
            std::list<Lattice::EdgeDescriptor> edges =
                lattice::getTopEdges(lattice, langCode, vertex);
            for (std::list<Lattice::EdgeDescriptor>::iterator edgeIt =
                    edges.begin();
                    edgeIt != edges.end(); ++ edgeIt) {
                std::vector<std::string> interpretation;

                for (std::vector<std::string>::iterator pattern_it = unifiedPatterns.begin();
                        pattern_it != unifiedPatterns.end(); ++ pattern_it) {

                    AnnotationItem annotationItem =
                        lattice.getEdgeAnnotationItem(*edgeIt);
                    if (lattice::isDiscarded(lattice, *edgeIt))
                        continue; //skip discarded edges
                    std::string morphology = lattice::getMorphologyString(
                            lattice, *edgeIt);

                    std::string slot;
                    std::string value;
                    if (RegExp::FullMatch(morphology, *pattern_it, &slot, &value)) {
                        interpretation.push_back(value);
                    } else {
                        interpretation.push_back("");
                    }
                }
                unifiedInterpretations.push_back(interpretation);

            }
            vertexI ++;
        }
    }

    return unifiedInterpretations;
}

std::set<std::string> UnifyAction::generateUnifiedInterpretationStrings(
        std::vector<std::vector<std::string> > unifiedInterpretations) {
    std::set<std::string> unifiedStrings;
    for (std::vector<std::vector<std::string> >::iterator interpIt =
            unifiedInterpretations.begin();
            interpIt != unifiedInterpretations.end();
            ++ interpIt) {
        std::string unifiedString = boost::algorithm::join(*interpIt, ":([^:]+:)*");
        unifiedStrings.insert(unifiedString);
    }
    return unifiedStrings;
}

std::vector<std::list<Lattice::EdgeDescriptor> >
UnifyAction::generateUnifiedEdgesList(Lattice &lattice, std::string langCode,
        RuleTokenSizes &ruleTokenSizes, int matchedStartIndex,
        std::set<std::string> unifiedStrings) {
    std::vector<std::list<Lattice::EdgeDescriptor> > edgesToKeep;
    for (std::set<std::string>::iterator uniPatternIt =
            unifiedStrings.begin();
            uniPatternIt != unifiedStrings.end();
            ++ uniPatternIt) {
        std::vector<std::list<Lattice::EdgeDescriptor> > matchingSequences;
        bool patternMatched = true;
        for (std::vector<int>::iterator index_it = tokenIndices.begin();
                index_it != tokenIndices.end(); ++ index_it) {

            int count, before;
            if (! util::getUnifyActionTokenParams(ruleTokenSizes, *index_it,
                        count, before) )
                continue;

            //Lattice::VertexDescriptor vertex = matchedStartIndex + before;
            Lattice::VertexDescriptor vertex = lattice::getVertex(lattice,
                    langCode, before, matchedStartIndex);
//            //@todo: czy to sprawdzenie jest nadal konieczne?
//            ta funkcja getVertex nie robi czegos takiego?
//            while (lattice::getTopEdges(lattice, vertex).size() == 0) {
//                before ++;
//                vertex = matchedStartIndex + before;
//            }

            int offset = vertex;
            int vertexI = 0;
            while (vertexI < count) {
                vertex = lattice::getVertex(lattice, langCode, vertexI, offset);
                std::list<Lattice::EdgeDescriptor> edges =
                    lattice::getTopEdges(lattice, langCode, vertex);
                std::vector<Lattice::EdgeDescriptor> matchingEdges;
                for (std::list<Lattice::EdgeDescriptor>::iterator edgeIt =
                        edges.begin();
                        edgeIt != edges.end(); ++ edgeIt) {
                    AnnotationItem annotationItem =
                        lattice.getEdgeAnnotationItem(*edgeIt);
                    if (lattice::isDiscarded(lattice, *edgeIt))
                        continue; //skip discarded edges
                    Morphology morphology = lattice::getMorphology(lattice, *edgeIt);
                    std::string morphologyString =
                        util::getMorphologyString(morphology);
                    if (RegExp::PartialMatch(morphologyString, *uniPatternIt)) {
                        matchingEdges.push_back(*edgeIt);
                    }
                }
                if (matchingEdges.empty()) {
                    patternMatched = false;
                    break;
                }
                if (! matchingSequences.empty()) {
                    std::vector<std::list<Lattice::EdgeDescriptor> > newMatchingSequences;
                    for (std::vector<std::list<
                            Lattice::EdgeDescriptor> >::iterator listIt =
                            matchingSequences.begin();
                            listIt != matchingSequences.end();
                            ++ listIt) {
                        for (std::vector<Lattice::EdgeDescriptor>::iterator edgeIt =
                                matchingEdges.begin();
                                edgeIt != matchingEdges.end();
                                ++ edgeIt) {
                            std::list<Lattice::EdgeDescriptor> tmpList = *listIt;
                            tmpList.push_back(*edgeIt);
                            newMatchingSequences.push_back(tmpList);
                        }
                    }
                    matchingSequences = newMatchingSequences;
                } else {
                    for (std::vector<Lattice::EdgeDescriptor>::iterator edgeIt =
                            matchingEdges.begin();
                            edgeIt != matchingEdges.end();
                            ++ edgeIt) {
                        std::list<Lattice::EdgeDescriptor> tmpList;
                        tmpList.push_back(*edgeIt);
                        matchingSequences.push_back(tmpList);
                    }
                }

                ++ vertexI;
            }
            if (! patternMatched)
                break;

        }
        if (patternMatched) {
            edgesToKeep.insert(edgesToKeep.end(), matchingSequences.begin(),
                    matchingSequences.end());
        }
    }
    return edgesToKeep;
}

bool UnifyAction::isUnifyingPossible(Lattice &lattice, std::string langCode,
        int matchedStartIndex, RuleTokenSizes &ruleTokenSizes) {

    bool toApply = true;
    std::vector<std::string>::iterator attribute_it = unifiedAttributes.begin();
    for (std::vector<std::string>::iterator pattern_it = unifiedPatterns.begin();
            pattern_it != unifiedPatterns.end(); ++ pattern_it) {

        bool wasAllNulls = false;

        std::set<std::string> refValues;
        for (std::vector<int>::iterator index_it = tokenIndices.begin();
                index_it != tokenIndices.end(); ++ index_it) {

            int count, before;
            if (! util::getUnifyActionTokenParams(ruleTokenSizes, *index_it,
                        count, before) )
                continue;

            //Lattice::VertexDescriptor vertex = matchedStartIndex + before;
            Lattice::VertexDescriptor vertex = lattice::getVertex(lattice,
                    langCode, before, matchedStartIndex);
//            //@todo: czy to sprawdzenie jest nadal konieczne?
//            ta funkcja getVertex nie robi czegos takiego?
//            while (lattice::getTopEdges(lattice, vertex).size() == 0) {
//                before ++;
//                vertex = matchedStartIndex + before;
//            }

            int offset = vertex;
            int vertexI = 0;
            while (vertexI < count) {
                vertex = lattice::getVertex(lattice, langCode, vertexI, offset);
                std::list<Lattice::EdgeDescriptor> edges =
                    lattice::getTopEdges(lattice, langCode, vertex);
                bool allValuesNull = true;
                std::set<std::string> values;
                for (std::list<Lattice::EdgeDescriptor>::iterator edgeIt =
                        edges.begin();
                        edgeIt != edges.end(); ++ edgeIt) {
                    AnnotationItem annotationItem =
                        lattice.getEdgeAnnotationItem(*edgeIt);
                    if (lattice::isDiscarded(lattice, *edgeIt))
                        continue; //skip discarded edges
                    std::string morphology = lattice::getMorphologyString(
                            lattice, *edgeIt);

                    std::string slot;
                    std::string value;
                    if (RegExp::FullMatch(morphology, *pattern_it, &slot, &value)) {
                        allValuesNull = false;
                        values.insert(value);
                    }
                }
                if (! values.empty()) {
                    if (refValues.empty()) {
                        refValues.insert(values.begin(), values.end());
                    } else {
                        std::set<std::string> intersection;
                        std::set<std::string>::iterator it;
                        std::set_intersection(refValues.begin(), refValues.end(),
                                values.begin(), values.end(),
                                std::inserter(intersection, intersection.begin())
                                );
                        refValues.clear();
                        refValues.insert(intersection.begin(), intersection.end());
                    }
                }
                if (allValuesNull && (! wasAllNulls)) {
                    wasAllNulls = true;
                }
                vertexI ++;
            }

        }
        if (! refValues.empty()) { //there are some common values of the attribute
            if (wasAllNulls) { //there is a token with no value of the attribute defined
                if (! nullAgreement)
                    toApply = false; //cannot apply
            }
        } else { //no common values of the attribute, cannot unify
            toApply = false;
        }
        if (! toApply)
            break;
        ++ attribute_it;
    }
    return toApply;
}

void UnifyAction::discardUnunifiedEdges(Lattice &lattice, std::string langCode,
        RuleTokenSizes &ruleTokenSizes, int matchedStartIndex,
        std::vector<std::list<Lattice::EdgeDescriptor> > edgesToKeep) {

    std::vector<std::set<std::string> > unifiedMorphoStrings;
    for (std::vector<int>::iterator indexIt = tokenIndices.begin();
            indexIt != tokenIndices.end(); ++ indexIt) {
        std::set<std::string> tmpSet;
        unifiedMorphoStrings.push_back(tmpSet);
    }

    for (std::vector<std::list<Lattice::EdgeDescriptor> >::iterator listIt =
            edgesToKeep.begin();
            listIt != edgesToKeep.end();
            ++ listIt) {
        int tokenCounter = 0;
        for (std::list<Lattice::EdgeDescriptor>::iterator edgeIt = listIt->begin();
                edgeIt != listIt->end(); ++ edgeIt) {
            std::string morphologyString = lattice::getMorphologyString(
                    lattice, *edgeIt);
            unifiedMorphoStrings[tokenCounter].insert(morphologyString);
            tokenCounter ++;
        }
    }

    int tokenCounter = 0;
    for (std::vector<int>::iterator index_it = tokenIndices.begin();
            index_it != tokenIndices.end(); ++ index_it) {

        int count, before;
        if (! util::getUnifyActionTokenParams(ruleTokenSizes, *index_it,
                    count, before) ) {
            tokenCounter ++;
            continue;
        }

        //Lattice::VertexDescriptor vertex = matchedStartIndex + before;
        Lattice::VertexDescriptor vertex = lattice::getVertex(lattice,
                langCode, before, matchedStartIndex);
//        //@todo: czy to sprawdzenie jest nadal konieczne?
//        ta funkcja getVertex nie robi czegos takiego?
//        while (lattice::getTopEdges(lattice, vertex).size() == 0) {
//            before ++;
//            vertex = matchedStartIndex + before;
//        }

        std::set<std::string> morphologyStrings =
            unifiedMorphoStrings[tokenCounter];

        int offset = vertex;
        int vertexI = 0;
        while (vertexI < count) {
            vertex = lattice::getVertex(lattice, langCode, vertexI, offset);
            std::list<Lattice::EdgeDescriptor> edges =
                lattice::getTopEdges(lattice, langCode, vertex);
            for (std::list<Lattice::EdgeDescriptor>::iterator edgeIt =
                    edges.begin();
                    edgeIt != edges.end(); ++ edgeIt) {
                std::string morphologyString = lattice::getMorphologyString(
                        lattice, *edgeIt);
                if (morphologyStrings.find(morphologyString) == morphologyStrings.end()) {
                    lattice.discard(*edgeIt);
                }
            }
            vertexI ++;
        }

        tokenCounter ++;
    }

}


}

}

}
