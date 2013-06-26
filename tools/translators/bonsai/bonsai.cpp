#include "bonsai.hpp"
#include "lattice_wrapper.hpp"

Bonsai::Bonsai(poleng::bonsai::TranslatorPtr translator, const std::string& langCode) :
    translator_(translator), langCode_(langCode)
{ }

void Bonsai::translate(Lattice &lattice, Lattice::EdgeDescriptor edge) {
    Lattice::VertexDescriptor start = lattice.getEdgeSource(edge);
    Lattice::VertexDescriptor end   = lattice.getEdgeTarget(edge);

    translator_->translate(lattice, start, end, langCode_);
}
