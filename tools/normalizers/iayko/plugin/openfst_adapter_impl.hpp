#ifndef OPENFST_ADAPTER_IMPL_HDR
#define OPENFST_ADAPTER_IMPL_HDR

#include "openfst_adapter_interface.hpp"

#include <fst/extensions/far/far.h>
#include <fst/fstlib.h>


class OpenFSTAdapterImpl : public OpenFSTAdapterInterface
{
public:
    OpenFSTAdapterImpl();
    virtual ~OpenFSTAdapterImpl();

    virtual void init(const std::string& far, const std::string& fst);
    virtual void init(std::vector< std::pair<std::string, std::string> > spec);

    virtual std::string normalize(const std::string& input);

private:
    static const size_t MAX_LINE_LENGTH;

    std::vector< const fst::Fst<fst::StdArc>* > automata_;
    const fst::Fst<fst::StdArc>* filter_;

    fst::StdVectorFst* compileByte_(const std::string& s);
    const fst::Fst<fst::StdArc>* loadFstFromFar_(
            const std::string& far,
            const std::string& fst);
    std::string fstToString_(fst::StdVectorFst& automaton);
};


#endif
