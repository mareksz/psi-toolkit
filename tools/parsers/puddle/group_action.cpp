#include "group_action.hpp"

#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

GroupAction::GroupAction(std::string aGroup, unsigned int aStart,
        unsigned int aEnd, unsigned int aHead,
        std::string aRuleName) {
    init(aGroup, aStart, aEnd, aHead, aRuleName);
}

bool GroupAction::apply(Lattice &lattice, std::string langCode,
        int matchedStartIndex, RuleTokenSizes &ruleTokenSizes,
        std::list<Lattice::EdgeSequence> &rulePartitions) {

    std::cout << "\tgroup action apply\n";
    std::cout << "\tmatched start index=" << matchedStartIndex << std::endl;
    int realStart;
    int realEnd;
    int realHead;
    if (! util::getGroupActionParams(ruleTokenSizes, start, head, end,
            realStart, realHead, realEnd)) {
        throw PuddleRuleElementNotMatchedException(
                "Group rule head " + boost::lexical_cast<std::string>(head)
                + " is empty"
                );
    }
    std::cout << "\t\truleTokenSizes: [";
    for (RuleTokenSizes::const_iterator i = ruleTokenSizes.begin(); i != ruleTokenSizes.end(); ++i)
        std::cout << *i << ' ';
    std::cout << "]\n";
    std::cout << "\tstart=" << start << ", end=" << end << ", head=" << head << std::endl;



    std::cout << "\trealStart=" << realStart << ", realEnd=" << realEnd << ", realHead=" << realHead << std::endl;
    std::cout << "\tgroup=" << this->group << std::endl;
    Lattice::VertexDescriptor startVertex = lattice::getVertex(
            lattice, langCode, realStart, matchedStartIndex + realStart);
    //Lattice::VertexDescriptor startVertex = matchedStartIndex + realStart;
    ////@todo: rozwiazanie tymczasowe. nie uwzglednia to chyba lewego kontekstu
    //Lattice::VertexDescriptor headVertex = lattice::getVertex(
    //        lattice, matchedStartIndex + realHead);
    Lattice::VertexDescriptor headVertex = lattice::getVertex(
            lattice, langCode, realHead, matchedStartIndex + realHead);
    //Lattice::VertexDescriptor headVertex = matchedStartIndex + realHead;
    ////@todo: rozwiazanie tymczasowe. nie uwzglednia to chyba lewego kontekstu
    //Lattice::VertexDescriptor endVertex = lattice::getVertex(
    //        lattice, matchedStartIndex + realEnd);
    Lattice::VertexDescriptor endVertex = lattice::getVertex(
            lattice, langCode, realEnd, matchedStartIndex+ realEnd);

    std::cout << "\tstartVertex=" << startVertex << std::endl;
    std::cout << "\theadVertex=" << headVertex << std::endl;
    std::cout << "\tendVertex=" << endVertex << std::endl;
    //Lattice::VertexDescriptor endVertex = matchedStartIndex + realEnd;
    ////@todo: rozwiazanie tymczasowe. nie uwzglednia to chyba lewego kontekstu
    std::list<Lattice::EdgeDescriptor> startEdges = lattice::getTopEdges(
            lattice, langCode, startVertex);
    std::list<Lattice::EdgeDescriptor> headEdges = lattice::getTopEdges(
            lattice, langCode, headVertex);
    std::list<Lattice::EdgeDescriptor> endEdges = lattice::getTopEdges(
            lattice, langCode, endVertex);
    if (startEdges.empty() || headEdges.empty() || endEdges.empty()) {
        return false;
    }
    //std::list<Lattice::EdgeSequence> groupPartitions =
    //    lattice::getEdgesRange(
    //        lattice, startVertex, endVertex
    //        );
    //int headEdgeIndex = realHead - realStart
    std::cout << "\tvertices got, adding parse edges\n";

    lattice::addParseEdges(
            lattice,
            langCode,
            startEdges,
            endEdges,
            this->group,
            headEdges,
            rulePartitions,
            realHead
            );

    return true;
}

bool GroupAction::test(Lattice &lattice, std::string, int,
        RuleTokenSizes &ruleTokenSizes,
        std::list<Lattice::EdgeSequence>&) {
    if ( ( (size_t) lattice.getLastVertex() ) < head) {
        return false;
    }
    if (ruleTokenSizes[head - 1] == 0) {
        throw PuddleRuleElementNotMatchedException(
                "Group head element " + boost::lexical_cast<std::string>(head)
                + " is empty"
                );
    }
    return true;
}

void GroupAction::init(std::string aGroup, unsigned int aStart,
        unsigned int aEnd, unsigned int aHead,
        std::string aRuleName) {
    group = aGroup;
    start = aStart;
    end = aEnd;
    head = aHead;
    if ((head > (end+1)) || (head <= start)) {
        if (start == end)
            head = 0;
        else
            head = (head % (end+1)) - start;
    }
    type = "group";
    ruleName = aRuleName;
}


}

}

}
