#ifndef RULEMATCHER_H__
#define RULEMATCHER_H__

#include <string>
#include <vector>

//#include "ParseGraph.hpp"
//#include "TransitionInfo.hpp"

#include "rule.hpp"
//#include "entity.hpp"
//#include "token.hpp"
//#include "group.hpp"
#include <boost/algorithm/string.hpp>

#include "puddle_util.hpp"

#include "tagset.hpp"

namespace poleng {

    namespace bonsai {

        namespace puddle {

            class RuleMatcher {
                public:
                    RuleMatcher();
                    ~RuleMatcher();
                    //ParseGraphPtr applyRules(std::string &sentence, Entities &entities, Edges &lattice);
                    //ParseGraphPtr applyRules(std::string &sentenceString, ParseGraphPtr inputGraph, Lattice &lattice);
                    void applyRules(std::string &sentenceString,
                            Lattice &lattice);

                    //void setRules(Rules aRules);
                    void setRules(RulesPtr aRules);

                    void setSyntok();
                    void setNoSyntok();
                    void setDisamb();
                    void setNoRepeats();

                    void setTagset(TagsetPtr tagset_) { tagset = tagset_; }

                private:
                    RulesPtr rules;
                    bool disamb; //@todo: do czego to jest?
                    bool norepeats;
                    bool syntok; //@todo: do czego to jest?

                    //void unescapeSpecialChars(Edges &edges);
                    //void unescapeSpecialChars(ParseGraphPtr pg, Lattice &lattice);
                    void unescapeSpecialChars(Lattice &lattice);
                    //void addPosEdges(Edges &edges);
                    //void addPosEdges(ParseGraphPtr pg, Lattice &lattice);
                    void addPosEdges(Lattice &lattice);

                    TagsetPtr tagset;
                    //std::string generateSentencePattern(Edges &edges);
                    //std::string generateSentenceString(ParseGraphPtr pg, Lattice &lattice);
                    std::string generateSentenceString(Lattice &lattice);
            };

        }

    }

}

#endif

