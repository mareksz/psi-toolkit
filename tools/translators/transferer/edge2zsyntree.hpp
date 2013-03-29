#pragma once

#include "lattice.hpp"
#include "zvalue.hpp"

class EdgeToZsyntreeConverter {

public:
    EdgeToZsyntreeConverter(Lattice& latticeArg);

    zsyntree* convertEdgeToZsyntree(
        Lattice::EdgeDescriptor start_edge);

private:
    zsyntree* generateEquivTree_(
        Lattice::EdgeDescriptor edge);

    zsyntree* generateEquivLeaf_(
        Lattice::EdgeDescriptor equivEdge);

    Lattice& lattice;

    zsymbolfactory* sym_fac;
    zobjects_holder* holder;

    LayerTagCollection lexemeTag_;
    LayerTagCollection formTag_;
    LayerTagMask equivMask_;
};
