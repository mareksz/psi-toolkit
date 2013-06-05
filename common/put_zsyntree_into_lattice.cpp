#include "put_zsyntree_into_lattice.hpp"

Lattice::EdgeDescriptor putZsyntreeIntoLattice(
    Lattice & lattice,
    LayerTagCollection targetTags,
    zsyntree* tree) {

    AnnotationItem annotationItem(
        tree->getCategory() ? tree->getCategory()->get_string() : "");
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

    Lattice::VertexDescriptor emergencyFromVertex = lattice.getLastVertex();
    Lattice::VertexDescriptor emergencyToVertex = lattice.getFirstVertex();

    bool wereSubedgesAdded = false;
    for (int i = 0; i <= tree->last_subtree; ++i) {
        if (!NULLP(tree->getSubtree(i)->getCategory())) {
            Lattice::EdgeDescriptor subedge = putZsyntreeIntoLattice(
                lattice,
                targetTags,
                tree->getSubtree(i));
            builder.addEdge(subedge, tree->getSubtree(i)->label);
            wereSubedgesAdded = true;

            if (lattice.getEdgeSource(subedge) < emergencyFromVertex) {
                emergencyFromVertex = lattice.getEdgeSource(subedge);
            }
            if (lattice.getEdgeTarget(subedge) > emergencyToVertex) {
                emergencyToVertex = lattice.getEdgeTarget(subedge);
            }
        }
    }

    Lattice::VertexDescriptor fromVertex = tree->segment_beg;
    Lattice::VertexDescriptor toVertex = tree->segment_beg + tree->segment_len;

    bool vertexSet = false;

    try {
        Lattice::EdgeDescriptor originalEdge
            = boost::any_cast<Lattice::EdgeDescriptor>(tree->getOrigin());

        fromVertex = lattice.getEdgeSource(originalEdge);
        toVertex = lattice.getEdgeTarget(originalEdge);
        vertexSet = true;

    } catch (const boost::bad_any_cast &) {
        ;
    }

    if (!wereSubedgesAdded) {
        try {
            Lattice::EdgeDescriptor originalEdge
                = boost::any_cast<Lattice::EdgeDescriptor>(tree->getOrigin());
            if (lattice.getEdgeAnnotationItem(originalEdge) != annotationItem) {
                builder.addEdge(originalEdge);

                if (lattice.getEdgeSource(originalEdge) < emergencyFromVertex) {
                    emergencyFromVertex = lattice.getEdgeSource(originalEdge);
                }
                if (lattice.getEdgeTarget(originalEdge) > emergencyToVertex) {
                    emergencyToVertex = lattice.getEdgeTarget(originalEdge);
                }
            }
        } catch (const boost::bad_any_cast &) {
            std::stringstream errorSs;
            errorSs << "Parser error: tree origin is not an edge (in tree of "
                    << (tree->getCategory() ? tree->getCategory()->get_string() : "(null)") << ")";
//            throw ParserException(errorSs.str());
        }
    }

    bool newEdge = false;

    if (!vertexSet && tree->segment_len == 0) {
        newEdge = true;

        if (emergencyFromVertex < emergencyToVertex) {
            fromVertex = emergencyFromVertex;
            toVertex = emergencyToVertex;
        } else {
            fromVertex = lattice.addLooseVertex();
            toVertex = lattice.addLooseVertex();
        }
    }

    Lattice::EdgeDescriptor addedEdge = lattice.addEdge(
        fromVertex,
        toVertex,
        annotationItem,
        targetTags,
        builder.build());

    if (newEdge)
        tree->setOrigin(addedEdge);

    return addedEdge;
}
