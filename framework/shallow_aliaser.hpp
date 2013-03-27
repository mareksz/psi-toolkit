#ifndef SHALLOW_ALIASER_HDR
#define SHALLOW_ALIASER_HDR

#include <string>
#include <map>
#include <vector>

class ShallowAliaser {

public:

    std::string replace(const std::string& pipeline);
    std::vector<std::string> replace(std::vector<std::string> pipeline);

private:

    static const std::map<std::string, std::string> aliases_;

    /*
    void initializeAliases_();
    void addWriteGetAlias(std::string, std::string);
    void addReadReaderAlias(std::string, std::string);
    void addWriteWriterGetAlias(std::string, std::string);
    void addNullDrawWriteAlias(std::string, std::string);
    */
};

#endif
