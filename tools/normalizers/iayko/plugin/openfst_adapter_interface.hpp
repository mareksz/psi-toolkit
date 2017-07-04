#ifndef OPENFST_ADAPTER_INTERFACE_HDR
#define OPENFST_ADAPTER_INTERFACE_HDR

#include <string>

#include "plugin_adapter.hpp"


class OpenFSTAdapterInterface : public PluginAdapter
{
public:
    virtual ~OpenFSTAdapterInterface()
    {
    }

    virtual void init(const std::string& far, const std::string& fst) = 0;

    virtual std::string normalize(const std::string& input) = 0;
};


#endif
