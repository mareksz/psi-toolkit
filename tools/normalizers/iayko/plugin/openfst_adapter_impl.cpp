#include "openfst_adapter_impl.hpp"


OpenFSTAdapterImpl::OpenFSTAdapterImpl()
{
}


OpenFSTAdapterImpl::~OpenFSTAdapterImpl()
{
}


std::string OpenFSTAdapterImpl::normalize(const std::string & input)
{
    if (input == "iayko") {
        return "jajko";
    }
    return input;
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
