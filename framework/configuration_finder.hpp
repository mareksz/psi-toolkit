#pragma once

#include "factories_keeper.hpp"

class ConfigurationFinder {
public:
    ConfigurationFinder(
        boost::filesystem::path startDirectory,
        FactoriesKeeper& factoryKeeper);

    void run();

private:
    void runOnSingleFile_(boost::filesystem::path filePath);

    boost::filesystem::path startDirectory_;
    FactoriesKeeper& factoryKeeper_;
};
