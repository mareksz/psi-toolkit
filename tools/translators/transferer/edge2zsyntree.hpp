#pragma once

#include "lattice.hpp"
#include "zvalue.hpp"

zsyntree* convertEdgeToZsyntree(
    Lattice& lattice,
    Lattice::EdgeDescriptor start_edge);
