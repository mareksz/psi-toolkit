#pragma once

#include "factories_keeper.hpp"

#include <boost/property_tree/ptree.hpp>

class ConfigurationFinder {
public:
    ConfigurationFinder(
        boost::filesystem::path startDirectory,
        FactoriesKeeper& factoryKeeper);

    void run();

private:
    static const boost::filesystem::path CONFIGURATION_FILE_NAME;

    void runOnSingleFile_(boost::filesystem::path filePath);
    void processConfiguration_(boost::property_tree::ptree& configurationNode);

    std::list<std::string> getListOfTags_(boost::property_tree::ptree& configurationNode);
    std::list<std::list<std::string> > getListOfListOfTags_(
        boost::property_tree::ptree& configurationNode);

    boost::filesystem::path startDirectory_;
    FactoriesKeeper& factoryKeeper_;
};
