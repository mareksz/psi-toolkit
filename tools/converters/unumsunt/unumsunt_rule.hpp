#ifndef UNUMSUNT_RULE_HDR
#define UNUMSUNT_RULE_HDR


#include <string>
#include <utility>
#include <vector>

#include "annotation_item_manager.hpp"


typedef std::pair< std::string, std::string > StringPair;


class UnumsuntRule {

public:
    UnumsuntRule() { }

    operator std::string() const;

    void addCondition(std::string arg, std::string val);
    void addCommand(std::string arg, std::string val);

    bool apply(AnnotationItemManager & manager, AnnotationItem & item);

private:
    std::vector<StringPair> conditions;
    std::vector<StringPair> commands;

};


#endif
