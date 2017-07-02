#include "openfst_adapter_impl.hpp"


OpenFSTAdapterImpl::OpenFSTAdapterImpl()
{
}


OpenFSTAdapterImpl::~OpenFSTAdapterImpl()
{
}


// ==============================================

extern "C" OpenFSTAdapterImpl* create()
{
    return new OpenFSTAdapterImpl;
}


extern "C" void destroy(OpenFSTAdapterImpl* Tl)
{
    delete Tl ;
}
