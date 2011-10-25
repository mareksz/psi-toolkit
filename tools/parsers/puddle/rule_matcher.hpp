#ifndef RULEMATCHER_H__
#define RULEMATCHER_H__

#include <string>
#include <vector>
#include "rule.hpp"
#include <boost/algorithm/string.hpp>
#include "puddle_util.hpp"
#include "tagset.hpp"

namespace poleng {

    namespace bonsai {

        namespace puddle {

            class RuleMatcher {
                public:
                    RuleMatcher();
//                    ~RuleMatcher();
                    void applyRules(Lattice &lattice);

                    void setRules(RulesPtr aRules);

                    void setSyntok();
                    void setNoSyntok();
                    void setNoRepeats();

                    void setTagset(TagsetPtr tagset_) { tagset = tagset_; }

                private:
                    void initProperties();

                    RulesPtr rules;
                    bool norepeats;
                    bool syntok; //@todo: do czego to jest?

                    void addPosEdges(Lattice &lattice);

                    TagsetPtr tagset;
                    std::string generateSentenceString(Lattice &lattice,
                            int startVertex = 0);
            };

            typedef boost::shared_ptr<RuleMatcher> RuleMatcherPtr;
        }

    }

}

#endif

