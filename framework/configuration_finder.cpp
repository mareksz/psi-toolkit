#include "configuration_finder.hpp"

ConfigurationFinder::ConfigurationFinder(
    boost::filesystem::path startDirectory,
    FactoriesKeeper& factoryKeeper)
    :startDirectory_(startDirectory), factoryKeeper_(factoryKeeper) {
}

void ConfigurationFinder::run() {
    INFO("looking in " << startDirectory_.string());

    boost::filesystem::recursive_directory_iterator end_itr;
    for (boost::filesystem::recursive_directory_iterator fiter(startDirectory_);
         fiter != end_itr;
         ++fiter) {
        boost::filesystem::path filename(fiter->path().filename());

        INFO("trying " << filename.string());
    }
}

void ConfigurationFinder::runOnSingleFile_(boost::filesystem::path /*filePath*/) {
}
