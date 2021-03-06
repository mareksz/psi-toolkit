#ifndef MAIN_FACTORIES_KEEPER_HDR
#define MAIN_FACTORIES_KEEPER_HDR

#include "factories_keeper.hpp"
#include <boost/program_options/options_description.hpp>

class MainFactoriesKeeper {

public:
    bool hasProcessorFactory(std::string processorName);
    ProcessorFactory& getProcessorFactory(std::string processorName);
    std::vector<std::string> getProcessorNames();
    std::set<std::string> getAliasNames();
    std::set<std::string> getAllAliases(std::string destination);

    std::list<ProcessorFactory*> getProcessorFactoriesForName(std::string name);

    // singleton pattern used here
    static MainFactoriesKeeper& getInstance();

private:
    MainFactoriesKeeper();

    // undefined as it is a singleton
    MainFactoriesKeeper(const MainFactoriesKeeper &);
    MainFactoriesKeeper& operator=(const MainFactoriesKeeper&);

    FactoriesKeeper keeper_;
};

#endif
