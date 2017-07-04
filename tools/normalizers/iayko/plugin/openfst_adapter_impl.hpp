#ifndef OPENFST_ADAPTER_IMPL_HDR
#define OPENFST_ADAPTER_IMPL_HDR

#include "openfst_adapter_interface.hpp"

#include <string>

#include <fst/extensions/far/far.h>
#include <fst/fstlib.h>


class OpenFSTAdapterImpl : public OpenFSTAdapterInterface
{
public:
    OpenFSTAdapterImpl();
    virtual ~OpenFSTAdapterImpl();

    virtual std::string normalize(const std::string& input);

private:
    fst::StdVectorFst* compileBYTE(std::string& s);
};


#endif
