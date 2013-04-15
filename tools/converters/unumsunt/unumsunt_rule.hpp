#ifndef UNUMSUNT_RULE_HDR
#define UNUMSUNT_RULE_HDR


#include <set>
#include <string>
#include <utility>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "annotation_item_manager.hpp"


typedef std::pair< std::string, std::string > StringPair;


class UnumsuntRule {

public:
    UnumsuntRule() : numberOfBreedCommands_(0) { }

    operator std::string() const;

    void addWord(std::string word);

    void addCondition(std::string arg, std::string val);

    /**
     * A rule cannot contain more than one alterantive ("breeding") command.
     * Method addCommand throws an error if tried to add second "breeding" command.
     */
    void addCommand(std::string arg, std::string val);

    /**
     * Remove all commands.
     */
    void clearCommands();

    bool apply(
        AnnotationItemManager & manager,
        std::vector< boost::shared_ptr<AnnotationItem> > & items);

private:
    std::set<std::string> words;
    std::vector<StringPair> conditions;
    std::vector<StringPair> commands;

    /**
     * A rule cannot contain more than one alterantive ("breeding") command.
     */
    int numberOfBreedCommands_;

    const static std::string CATEGORY_MARKER;
    const static char REFERENCE_MARKER;
    const static std::string ALTERNATIVE_SEPARATORS;
    const static std::string ANY_DESIGNATION;
    const static std::string FALSE_DESIGNATION;

};


#endif
