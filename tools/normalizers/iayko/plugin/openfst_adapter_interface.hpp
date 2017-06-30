#ifndef OPENFST_ADAPTER_INTERFACE_HDR
#define OPENFST_ADAPTER_INTERFACE_HDR

#include <string>
#include <list>

#include "plugin_adapter.hpp"

typedef std::list<std::string> SuggestionsList;


class OpenFSTAdapterInterface : public PluginAdapter
{
public:
    virtual ~OpenFSTAdapterInterface()
    {
    }
};


#endif
