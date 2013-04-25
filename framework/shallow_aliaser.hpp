#ifndef SHALLOW_ALIASER_HDR
#define SHALLOW_ALIASER_HDR

#include <string>
#include <map>
#include <vector>
#include <set>

class ShallowAliaser {

public:

    ShallowAliaser();

    std::string replace(const std::string& pipeline);
    std::vector<std::string> replace(std::vector<std::string> pipeline);

    void addReadReaderAlias(std::string, std::string);
    void addWriteWriterGetAlias(std::string, std::string);
    void addWriteGetAlias(std::string, std::string);
    void addWriteGetDrawAlias(std::string, std::string);

    static std::set<std::string> getAllAliases();
    static bool hasAlias(const std::string&);
    static std::string getProcessorNameForAlias(const std::string& alias);

private:

    static std::map<std::string, std::string> ALIASES;
    void addAlias_(std::string, std::string);
};

#endif
