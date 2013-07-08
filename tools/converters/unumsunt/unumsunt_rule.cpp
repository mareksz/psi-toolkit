#include "unumsunt_rule.hpp"


#include <sstream>

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

#include "exceptions.hpp"
#include "zvalue.hpp"


const std::string UnumsuntRule::CATEGORY_MARKER = "CAT";
const char UnumsuntRule::REFERENCE_MARKER = '$';
const std::string UnumsuntRule::ALTERNATIVE_SEPARATORS = "|";
const std::string UnumsuntRule::ANY_DESIGNATION = "0";
const std::string UnumsuntRule::FALSE_DESIGNATION = "-1";


UnumsuntRule::operator std::string() const {
    std::stringstream sstr;
    {
        std::string comma(" for ");
        BOOST_FOREACH(std::string word, words) {
            sstr << comma << "[" << word << "]";
            comma = " or ";
        }
    }
    {
        std::string comma(" if ");
        BOOST_FOREACH(StringPair condition, conditions) {
            sstr << comma << "[" << condition.first << "] == [" << condition.second << "]";
            comma = " and ";
        }
    }
    {
        std::string comma(" then ");
        if (isSkipRule_) {
            sstr << comma << "SKIP";
        } else {
            BOOST_FOREACH(StringPair command, commands) {
                sstr << comma << "[" << command.first << "] := [" << command.second << "]";
                comma = ", ";
            }
        }
    }
    return sstr.str();
}


void UnumsuntRule::addWord(std::string word) {
    words.insert(word);
}


void UnumsuntRule::addCondition(std::string arg, std::string val) {
    conditions.push_back(StringPair(arg, val));
}


void UnumsuntRule::addCommand(std::string arg, std::string val) {
    if (isSkipRule_) {
        std::stringstream errorSs;
        errorSs << "Tagset converter error: cannot add another command to the SKIP rule.";
        throw TagsetConverterException(errorSs.str());
    }
    if (val.find_first_of(ALTERNATIVE_SEPARATORS) != std::string::npos) {
        if (numberOfBreedCommands_ == 0) {
            numberOfBreedCommands_ = 1;
        } else {
            std::stringstream errorSs;
            errorSs << "Tagset converter error: wrong command."
                << "\nConverter rule cannot contain more than one alternative command."
                << "\nPlease split the rule into separate rules.";
            throw TagsetConverterException(errorSs.str());
        }
    }
    commands.push_back(StringPair(arg, val));
}


void UnumsuntRule::makeSkip() {
    commands.clear();
    numberOfBreedCommands_ = 0;
    isSkipRule_ = true;
}


void UnumsuntRule::clearCommands() {
    commands.clear();
    numberOfBreedCommands_ = 0;
    isSkipRule_ = false;
}


bool UnumsuntRule::apply(
    AnnotationItemManager & manager,
    std::vector< boost::shared_ptr<AnnotationItem> > & items
) {
    bool result = false;
    for (size_t i = 0; i < items.size(); ++i) {
        boost::shared_ptr<AnnotationItem> item = items[i];
        if (!item) {
            continue;
        }
        if (!words.empty() && !words.count(item->getText().substr(0, item->getText().find('+')))) {
            continue;
        }
        bool allConditionsSatisfied = true;
        BOOST_FOREACH(StringPair condition, conditions) {
            if (condition.first == CATEGORY_MARKER) {
                if (condition.second != item->getCategory()) {
                    allConditionsSatisfied = false;
                    break;
                }
            } else {
                if (manager.getValueAsString(*item, condition.first) != condition.second) {
                    allConditionsSatisfied = false;
                    break;
                }
            }
        }
        if (!allConditionsSatisfied) {
            continue;
        }
        if (isSkipRule_) {
            items[i].reset();
            result = true;
        }
        BOOST_FOREACH(StringPair command, commands) {
            std::vector<std::string> alternativeValues;
            boost::split(
                alternativeValues,
                command.second,
                boost::is_any_of(ALTERNATIVE_SEPARATORS));
            for (size_t i = 0; i < alternativeValues.size(); ++i) {
                boost::shared_ptr<AnnotationItem> itemCopy;
                if (i == 0) {
                    itemCopy = item;
                } else {
                    items.push_back(boost::shared_ptr<AnnotationItem>(
                        new AnnotationItem(*item)));
                    itemCopy = items.back();
                }
                if (command.first == CATEGORY_MARKER) {
                    if (command.second[0] == REFERENCE_MARKER) {
                        manager.setCategory(*itemCopy,
                            manager.getValueAsString(*itemCopy, alternativeValues[i].substr(1)));
                    } else {
                        manager.setCategory(*itemCopy, alternativeValues[i]);
                    }
                } else {
                    if (alternativeValues[i][0] == REFERENCE_MARKER) {
                        manager.setValue(*itemCopy, command.first,
                            manager.getValue(*itemCopy, alternativeValues[i].substr(1)));
                    } else if (alternativeValues[i] == ANY_DESIGNATION) {
                        manager.setValue(*itemCopy, command.first, manager.any_value());
                    } else if (alternativeValues[i] == FALSE_DESIGNATION) {
                        manager.setValue(*itemCopy, command.first, manager.false_value());
                    } else {
                        manager.setValue(*itemCopy, command.first, alternativeValues[i]);
                    }
                }
            }
        }
        result = true;
    }
    return result;
}
