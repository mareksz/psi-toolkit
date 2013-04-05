#ifndef SHALLOW_ALIASER_HDR
#define SHALLOW_ALIASER_HDR

#include <string>
#include <map>
#include <vector>

class ShallowAliaser {

public:

    ShallowAliaser();

    std::string replace(const std::string& pipeline);
    std::vector<std::string> replace(std::vector<std::string> pipeline);

    void addReadReaderAlias(std::string, std::string);
    void addWriteWriterGetAlias(std::string, std::string);
    void addWriteGetAlias(std::string, std::string);
    void addWriteGetDrawAlias(std::string, std::string);

private:

    std::map<std::string, std::string> aliases_;

    void addAlias_(std::string, std::string);
    void initializeAliases_();
};

#endif
