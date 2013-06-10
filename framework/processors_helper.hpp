#ifndef PROCESSORS_HELPER_HDR
#define PROCESSORS_HELPER_HDR

#include <string>
#include <vector>

class ProcessorsHelper {

public:
    static std::vector<std::string> getAllProcessorTypes_();
    static std::vector<std::string> getAllSubTypesForProcessorType_(
        const std::string& type);
    static std::vector<std::string> getAllProcessorsNamesForTypeAndSubType_(
        const std::string& type, const std::string& subtype);
};

#endif
