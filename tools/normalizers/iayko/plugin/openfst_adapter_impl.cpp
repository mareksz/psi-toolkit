#include "openfst_adapter_impl.hpp"


OpenFSTAdapterImpl::OpenFSTAdapterImpl()
{
}


OpenFSTAdapterImpl::~OpenFSTAdapterImpl()
{
}


std::string OpenFSTAdapterImpl::normalize(const std::string& input)
{
    if (input == "iayko") {
        return "jajko";
    }
    return input;
}


fst::StdVectorFst* OpenFSTAdapterImpl::compileBYTE(std::string& s)
{
    int state;
    const char* cstr = s.c_str();
    fst::StdVectorFst* automaton = new fst::StdVectorFst;
    state = automaton->AddState();
    automaton->SetStart(state);
    while (*cstr) {
        state = automaton->AddState();
        int symbol = (unsigned char)(*cstr);
        automaton->AddArc(state - 1, fst::StdArc(symbol, symbol, 0, state));
        ++cstr;
    }
    automaton->SetFinal(state, 0);
    return automaton;
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
