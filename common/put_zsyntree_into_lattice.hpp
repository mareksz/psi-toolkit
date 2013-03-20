#pragma once

#include "lattice.hpp"
#include "zvalue.hpp"

Lattice::EdgeDescriptor putZsyntreeIntoLattice(
    Lattice & lattice,
    LayerTagCollection targetTags,
    zsyntree* tree);
