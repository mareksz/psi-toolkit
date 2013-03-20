#include "put_zsyntree_into_lattice.hpp"

Lattice::EdgeDescriptor putZsyntreeIntoLattice(
    Lattice & lattice,
    LayerTagCollection targetTags,
    zsyntree* tree) {

    AnnotationItem annotationItem(tree->getCategory()->get_string());
    int block_size = 0;
    zvalue * block = tree->getAll(block_size);
    for (int i = 0; i < block_size; ++i) {
        if (!NULLP(block[i]) && !DELETEDP(block[i])) {
            zpair * zp = ZPAIRC(block[i]);
            if (!ZSYNTREEP(zp->getSecond())) {
                lattice.getAnnotationItemManager().setValue(
                    annotationItem,
                    zvalue_to_string(zp->getFirst()),
                    zp->getSecond());
            }
        }
    }

    Lattice::EdgeSequence::Builder builder(lattice);
    for (int i = 0; i <= tree->last_subtree; ++i) {
        Lattice::EdgeDescriptor subedge = putZsyntreeIntoLattice(
            lattice,
            targetTags,
            tree->getSubtree(i));
        builder.addEdge(subedge, tree->getSubtree(i)->label);
    }

    if (tree->last_subtree < 0) {
        try {
            Lattice::EdgeDescriptor originalEdge
                = boost::any_cast<Lattice::EdgeDescriptor>(tree->getOrigin());
            if (lattice.getEdgeAnnotationItem(originalEdge) != annotationItem) {
                builder.addEdge(originalEdge);
            }
        } catch (const boost::bad_any_cast &) {
            std::stringstream errorSs;
            errorSs << "Parser error: tree origin is not an edge (in tree of "
                << tree->getCategory()->get_string() << ")";
            throw ParserException(errorSs.str());
        }
    }

    return lattice.addEdge(
        tree->segment_beg,
        tree->segment_beg + tree->segment_len,
        annotationItem,
        targetTags,
        builder.build());

}
