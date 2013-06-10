#include <algorithm>

#include "processors_helper.hpp"
#include "main_factories_keeper.hpp"
#include "logging.hpp"

std::vector<std::string> ProcessorsHelper::getAllProcessorTypes_() {
    std::vector<std::string> types;
    std::string type;

    BOOST_FOREACH(std::string name, MainFactoriesKeeper::getInstance().getProcessorNames()) {
        type = MainFactoriesKeeper::getInstance().getProcessorFactory(name).getType();

        if (std::find(types.begin(), types.end(), type) == types.end()) {
            types.push_back(type);
        }
    }

    return types;
}

std::vector<std::string> ProcessorsHelper::getAllSubTypesForProcessorType_(
        const std::string& type) {

    std::vector<std::string> subtypes;
    std::string subtype;

    BOOST_FOREACH(std::string name, MainFactoriesKeeper::getInstance().getProcessorNames()) {
        if (MainFactoriesKeeper::getInstance().getProcessorFactory(name).getType() != type) {
            continue;
        }

        subtype = MainFactoriesKeeper::getInstance().getProcessorFactory(name).getSubType();
        if (std::find(subtypes.begin(), subtypes.end(), subtype) == subtypes.end()) {
            subtypes.push_back(subtype);
        }
    }

    std::sort(subtypes.begin(), subtypes.end());
    return subtypes;
}

std::vector<std::string> ProcessorsHelper::getAllProcessorsNamesForTypeAndSubType_(
        const std::string& type, const std::string& subtype) {
    std::vector<std::string> processors;

    BOOST_FOREACH(std::string name, MainFactoriesKeeper::getInstance().getProcessorNames()) {
        if (MainFactoriesKeeper::getInstance().getProcessorFactory(name).getType() != type ||
            MainFactoriesKeeper::getInstance().getProcessorFactory(name).getSubType() != subtype) {
            continue;
        }
        processors.push_back(name);
    }

    return processors;
}
