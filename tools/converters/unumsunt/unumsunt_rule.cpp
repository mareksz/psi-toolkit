#include "unumsunt_rule.hpp"


#include <sstream>

#include <boost/foreach.hpp>


UnumsuntRule::operator std::string() const {
    std::stringstream sstr;
    {
        std::string comma("if");
        BOOST_FOREACH(StringPair condition, conditions) {
            sstr << comma << " [" << condition.first << "] == [" << condition.second << "]";
            comma = " and";
        }
    }
    {
        std::string comma(" then");
        BOOST_FOREACH(StringPair command, commands) {
            sstr << comma << " [" << command.first << "] := [" << command.second << "]";
            comma = ",";
        }
    }
    return sstr.str();
}


void UnumsuntRule::addCondition(std::string arg, std::string val) {
    conditions.push_back(StringPair(arg, val));
}


void UnumsuntRule::addCommand(std::string arg, std::string val) {
    commands.push_back(StringPair(arg, val));
}


bool UnumsuntRule::apply(AnnotationItemManager & manager, AnnotationItem & item) {
    bool doesSatisfyAllConditions = true;
    BOOST_FOREACH(StringPair condition, conditions) {
        if (condition.first == "CAT") {
            if (condition.second != item.getCategory()) {
                doesSatisfyAllConditions = false;
                break;
            }
        } else {
            if (manager.getValueAsString(item, condition.first) != condition.second) {
                doesSatisfyAllConditions = false;
                break;
            }
        }
    }
    if (doesSatisfyAllConditions) {
        BOOST_FOREACH(StringPair command, commands) {
            if (command.first == "CAT") {
                if (command.second[0] == '$') {
                    manager.setCategory(item,
                        manager.getValueAsString(item, command.second.substr(1)));
                } else {
                    manager.setCategory(item, command.second);
                }
            } else {
                if (command.second[0] == '$') {
                    manager.setValue(item, command.first,
                        manager.getValue(item, command.second.substr(1)));
                } else {
                    manager.setValue(item, command.first, command.second);
                }
            }
        }
    }
    return doesSatisfyAllConditions;
}
