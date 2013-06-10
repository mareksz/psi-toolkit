#include "string_inflection.hpp"

#include <boost/foreach.hpp>
#include <boost/algorithm/string/predicate.hpp>

std::string pluralizeRegularWord(const std::string& word) {
    if (boost::algorithm::ends_with(word, "y")) {
        return word.substr(0, word.size() - 1) + "ies";
    }
    if (boost::algorithm::ends_with(word, "is")) {
        return word.substr(0, word.size() - 2) + "es";
    }
    if (boost::algorithm::ends_with(word, "s")
        || boost::algorithm::ends_with(word, "x")
        || boost::algorithm::ends_with(word, "sh")
        || boost::algorithm::ends_with(word, "ch")) {
        return word + "es";
    }
    if (boost::algorithm::ends_with(word, "z")) {
        return word.substr(0, word.size()) + "zes";
    }
    if (boost::algorithm::ends_with(word, "an")) {
        return word.substr(0, word.size() - 2) + "en";
    }
    return word + "s";
}
