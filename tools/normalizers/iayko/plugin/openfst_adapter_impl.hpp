#ifndef OPENFST_ADAPTER_IMPL_HDR
#define OPENFST_ADAPTER_IMPL_HDR

#include "openfst_adapter_interface.hpp"

#include <string>

class OpenFSTAdapterImpl : public OpenFSTAdapterInterface
{
public:
    OpenFSTAdapterImpl();
    virtual ~OpenFSTAdapterImpl();

    virtual std::string normalize(const std::string & input);
};

#endif
