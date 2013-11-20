#include "lattice.hpp"

#include <sstream>

#include "string_helpers.hpp"


const std::string Lattice::SYMBOL_TAG_NAME = "symbol";
const std::string Lattice::DISCARDED_TAG_NAME = "discarded";
const std::string Lattice::LEMMA_TAG_NAME = "lemma";
const std::string Lattice::TOKEN_TAG_NAME = "token";


Lattice::Lattice(AnnotationItemManager & annotationItemManager) :
    annotationItemManager_(annotationItemManager),
    nLooseVertices_(0),
    symbolTag_(layerTagManager_.createSingletonTagCollection(SYMBOL_TAG_NAME)),
    discardedTag_(layerTagManager_.createSingletonTagCollection(DISCARDED_TAG_NAME)),
    symbolMask_(layerTagManager_.getSingletonMask(SYMBOL_TAG_NAME)),
    tokenMask_(layerTagManager_.getSingletonMask(TOKEN_TAG_NAME))
{
    resizeImplicitEdgesStructures_();
}

Lattice::Lattice(AnnotationItemManager & annotationItemManager, const std::string & text) :
    annotationItemManager_(annotationItemManager),
    nLooseVertices_(0),
    symbolTag_(layerTagManager_.createSingletonTagCollection(SYMBOL_TAG_NAME)),
    discardedTag_(layerTagManager_.createSingletonTagCollection(DISCARDED_TAG_NAME)),
    symbolMask_(layerTagManager_.getSingletonMask(SYMBOL_TAG_NAME)),
    tokenMask_(layerTagManager_.getSingletonMask(TOKEN_TAG_NAME))
{
    appendString(text);
}

Lattice::~Lattice() { }

void Lattice::appendString(const std::string & text) {
    if (allText_.empty()) {
        allText_ = text;
    } else {
        allText_ += text;
    }

    resizeImplicitEdgesStructures_();
}

void Lattice::addSymbols(VertexDescriptor startVertex, VertexDescriptor endVertex) {
    if (isLooseVertex(startVertex) || isLooseVertex(endVertex)) {
        throw WrongVertexException("Cannot add default symbol edges between loose vertices.");
    }
    std::string::iterator iter = allText_.begin() + startVertex;
    std::string::iterator end = allText_.begin() + endVertex;
    VertexDescriptor vd = startVertex;
    while (iter != end) {
        std::string symbol;
        utf8::append(utf8::next(iter, end), std::back_inserter(symbol));
        addEdge(
            vd,
            vd + symbol.length(),
            AnnotationItem("'" + symbol),
            getSymbolTag());
        vd += symbol.length();
    }
}

void Lattice::appendStringWithSymbols(const std::string & text) {
    Lattice::VertexDescriptor joinPoint = getLastVertex();
    appendString(text);
    addSymbols(joinPoint, getLastVertex());
}

Lattice::VertexDescriptor Lattice::addLooseVertex() {
    ++nLooseVertices_;
    Graph::vertex_descriptor vertex = boost::add_vertex(VertexEntry(-nLooseVertices_), graph_);
    vertices_[-nLooseVertices_] = vertex;
    for (size_t i = 0; i < indexedTagMasks_.size(); ++i) {
        graph_[vertex].outEdgesIndex.push_back(
            std::list<EdgeDescriptorWrapperToFoolBoost146OrGnu461>()
        );
        graph_[vertex].inEdgesIndex.push_back(
            std::list<EdgeDescriptorWrapperToFoolBoost146OrGnu461>()
        );
    }
    return -nLooseVertices_;
}

Lattice::VertexDescriptor Lattice::getVertexForRawCharIndex(int ix) const {
    if (ix < 0) {
        throw NoVertexException("Raw char index below zero is incorrect.");
    }
    if (ix > int(allText_.length())) {
        throw NoVertexException("Raw char index above text length is incorrect.");
    }
    return ix;
}

Lattice::VertexDescriptor Lattice::getFirstVertex() {
    if (allInEdges(0).hasNext()) {
        VertexIterator vi(*this);
        return vi.next();
    }
    return 0;
}

Lattice::VertexDescriptor Lattice::getLastVertex() {
    if (allOutEdges(allText_.length()).hasNext()) {
        VertexIterator vi(*this);
        VertexDescriptor vd;
        while (vi.hasNext()) {
            vd = vi.next();
        }
        return vd;
    }
    return allText_.length();
}

size_t Lattice::getVertexRawCharIndex(VertexDescriptor vd) {
    if (isLooseVertex(vd)) {
        throw WrongVertexException("Loose vertices have no raw char index");
    }
    return size_t(vd);
}

Lattice::EdgeDescriptor Lattice::addEdge(
    VertexDescriptor from,
    VertexDescriptor to,
    const AnnotationItem& annotationItem,
    LayerTagCollection tags,
    EdgeSequence sequence,
    Score score,
    int ruleId,
    int treeChoice)
{
    // inheriting plane tags
    if (getLayerTagManager().onlyPlaneTags(tags).isEmpty())
        tags = createUnion(tags, sequence.gatherPlaneTags(*this));

    if (from == to) {
        throw LoopEdgeException("Cannot add a loop edge");
    }

    // doesn't check if a loose edge is reversed!
    if (from >= 0 && to >= 0 && from > to) {
        throw ReversedEdgeException("Cannot add a reversed edge");
    }

    // result of adding edge to the boost graph
    std::pair<Graph::edge_descriptor, bool> result;

    // a key for the edge counter hash
    std::pair<VertexDescriptor, VertexDescriptor> vpair(from, to);

    // a key for the VVC hash
    std::pair<
        std::pair<VertexDescriptor, VertexDescriptor>,
        AnnotationItem
    > hkey(vpair, annotationItem);

    std::vector<EdgeDescriptor> vectorOfOneEmptyEdgeDescriptor;
    // vectorOfOneEmptyEdgeDescriptor.push_back(EdgeDescriptor());

    // firstly, we add an empty edge descriptor to the VVC hash,
    // the proper edge addition will follow later
    std::pair<VVCHash::iterator, bool> insertResult(vvcHash_.insert(
        std::pair<
            std::pair<
                std::pair<VertexDescriptor, VertexDescriptor>,
                AnnotationItem
            >,
            std::vector<EdgeDescriptor>
        >(hkey, vectorOfOneEmptyEdgeDescriptor)
    ));

    // indicates if we need to add edge to the boost graph
    bool needToAddEdge = false;

    // finds an existing edge in the same plane
    EdgeDescriptor isoplanarEdge;
    bool isoplanarEdgeFound = false;
    BOOST_FOREACH(EdgeDescriptor edge, (insertResult.first)->second) {
        if (layerTagManager_.areInTheSamePlane(tags, getEdgeLayerTags(edge))) {
            isoplanarEdge = edge;
            isoplanarEdgeFound = true;
            break;
        }
    }

    // if there were no "such" edges in the VVC hash, we should add edge to the boost graph;
    // otherwise we shall merge the new edge with the existing edge
    // (unless the edges are in differet planes)
    if (insertResult.second || !isoplanarEdgeFound) {
        needToAddEdge = true;
    } else {
        EdgeDescriptor edge = isoplanarEdge;
        EdgeSequence::Iterator sequenceIter(*this, sequence);
        while (sequenceIter.hasNext()) {
            if (sequenceIter.next() == edge) {
                std::stringstream errorSs;
                errorSs << "Cannot add an edge referencing itself: "
                    << annotationItem.getText() << "/" << annotationItem.getCategory();
                throw EdgeSelfReferenceException(errorSs.str());
            }
        }
        LayerTagCollection oldTags = getEdgeLayerTags(edge);
        Score oldScore = getEdgeScore(edge);
        if (tags != oldTags) {
            tags = createUnion(oldTags, tags);
        }
        if (edge.isExplicit()) {
            if (tags != oldTags) {
                graph_[edge.descriptor].tagList = tags;
            }
            if (score > oldScore) {
                graph_[edge.descriptor].score = score;
            }
            graph_[edge.descriptor].partitions.push_back(
                Partition(tags, sequence, score, ruleId, treeChoice));
        } else {
            implicitOutEdges_.set(from, false);
            edge.implicitIndex = -1;
            needToAddEdge = true;
        }
    }

    if (needToAddEdge) {

        // updating the edge counter hash
        if (!edgeCounterHash_[vpair]) {
            edgeCounterHash_[vpair] = 1;
        } else {
            ++edgeCounterHash_[vpair];
        }

        // updating hidden edges, where applicable
        if (tags == getSymbolTag()) {
            try {
                firstOutEdge(from, symbolMask_);
                visibleImplicitOutEdges_[from] = true;
                EdgeDescriptor ed = firstInEdge(to, symbolMask_);
                visibleImplicitOutEdges_[getEdgeSource(ed)] = true;
            } catch (NoEdgeException) {
            }
        }

        // updating implicit edges, where applicable
        if (
            !isLooseVertex(from)
            && !isLooseVertex(to)
            && tags == getSymbolTag()
            && from + (int) symbolLength_(from) == to
            && !implicitOutEdges_[from]
        ) {
            implicitOutEdges_.set(from, true);
            (insertResult.first)->second.push_back(EdgeDescriptor(from));
            return EdgeDescriptor(from);
        }

        // the real addition of the edge to the graph

        Graph::vertex_descriptor boost_from;
        Graph::vertex_descriptor boost_to;

        VerticesMap::iterator iterFrom = vertices_.find(from);
        VerticesMap::iterator iterTo = vertices_.find(to);

        if (iterFrom != vertices_.end()) {
            boost_from = (*iterFrom).second;
        } else {
            boost_from = boost::add_vertex(VertexEntry(from), graph_);
            vertices_[from] = boost_from;
            for (size_t i = 0; i < indexedTagMasks_.size(); ++i) {
                graph_[boost_from].outEdgesIndex.push_back(
                    std::list<EdgeDescriptorWrapperToFoolBoost146OrGnu461>()
                );
                graph_[boost_from].inEdgesIndex.push_back(
                    std::list<EdgeDescriptorWrapperToFoolBoost146OrGnu461>()
                );
            }
        }

        if (iterTo != vertices_.end()) {
            boost_to = (*iterTo).second;
        } else {
            boost_to = boost::add_vertex(VertexEntry(to), graph_);
            vertices_[to] = boost_to;
            for (size_t i = 0; i < indexedTagMasks_.size(); ++i) {
                graph_[boost_to].outEdgesIndex.push_back(
                    std::list<EdgeDescriptorWrapperToFoolBoost146OrGnu461>()
                );
                graph_[boost_to].inEdgesIndex.push_back(
                    std::list<EdgeDescriptorWrapperToFoolBoost146OrGnu461>()
                );
            }
        }

        result = boost::add_edge(
            boost_from,
            boost_to,
            EdgeEntry(
                annotationItem,
                tags, score,
                Partition(tags, sequence, score, ruleId, treeChoice)
            ),
            graph_
        );

        if (result.second) {
            for (
                TagMasksHash::iterator iter = indexedTagMasks_.begin();
                iter != indexedTagMasks_.end();
                ++iter
            ) {
                if (matches(tags, iter->first)) {
                    graph_[boost_from].outEdgesIndex[iter->second].push_back(result.first);
                    graph_[boost_to].inEdgesIndex[iter->second].push_back(result.first);
                }
            }
            if (!isLooseVertex(from) && !isLooseVertex(to)) {
                for (VertexDescriptor vd = from; vd < to; ++vd) {
                    hiddenImplicitOutEdges_[vd] = true;
                }
            }
            (insertResult.first)->second.push_back(EdgeDescriptor(result.first));
            return EdgeDescriptor(result.first);
        }

    }

    // if there were no need for sdding new edge to the graph,
    // the edge on the same plane is returned
    if (isoplanarEdgeFound) {
        return isoplanarEdge;
    }

    throw EdgeInsertionException("An error occured while adding edge");

}

Lattice::EdgeDescriptor Lattice::addPartitionToEdge(
    EdgeDescriptor edge,
    LayerTagCollection tags,
    EdgeSequence sequence,
    Score score,
    int ruleId,
    int treeChoice) {
    // FIXME - ineffective! addEdge must be refactored

    LayerTagCollection edgePlaneTags =
        getLayerTagManager().onlyPlaneTags(getEdgeLayerTags(edge));

    return addEdge(getEdgeSource(edge),
                   getEdgeTarget(edge),
                   getEdgeAnnotationItem(edge),
                   createUnion(tags, edgePlaneTags),
                   sequence,
                   score,
                   ruleId,
                   treeChoice);
}

void Lattice::discard(EdgeDescriptor edge) {
    addPartitionToEdge(edge, discardedTag_, EdgeSequence(), -(std::numeric_limits<Score>::max)());
}

bool Lattice::isDiscarded(EdgeDescriptor edge) {
    return layerTagManager_.isThere(DISCARDED_TAG_NAME, getEdgeLayerTags(edge));
}

Lattice::InOutEdgesIterator Lattice::outEdges(
    Lattice::VertexDescriptor vertex,
    LayerTagMask mask
) {
    if (mask.isAny()) {
        return allOutEdges(vertex);
    }
    int ix = addTagMaskIndex_(mask);
    if (isLooseVertex(vertex)) {
        Graph::vertex_descriptor boost_vertex = vertices_[vertex];
        return Lattice::InOutEdgesIterator(
            graph_[boost_vertex].outEdgesIndex[ix].begin(),
            graph_[boost_vertex].outEdgesIndex[ix].end(),
            -1
        );
    }
    VerticesMap::iterator iter = vertices_.find(vertex);
    if (iter == vertices_.end()) {
        return Lattice::InOutEdgesIterator(
            (layerTagManager_.canBeAppliedToImplicitSymbol(mask)
             && implicitOutEdges_[vertex]) ?
                vertex : -1
        );
    }
    Graph::vertex_descriptor boost_vertex = (*iter).second;
    return Lattice::InOutEdgesIterator(
        graph_[boost_vertex].outEdgesIndex[ix].begin(),
        graph_[boost_vertex].outEdgesIndex[ix].end(),
        (layerTagManager_.canBeAppliedToImplicitSymbol(mask)
         && implicitOutEdges_[vertex]) ?
            vertex : -1
    );
}

Lattice::InOutEdgesIterator Lattice::inEdges(
    Lattice::VertexDescriptor vertex,
    LayerTagMask mask
) {
    if (mask.isAny()) {
        return allInEdges(vertex);
    }
    int ix = addTagMaskIndex_(mask);
    if (isLooseVertex(vertex)) {
        Graph::vertex_descriptor boost_vertex = vertices_[vertex];
        return Lattice::InOutEdgesIterator(
            graph_[boost_vertex].inEdgesIndex[ix].begin(),
            graph_[boost_vertex].inEdgesIndex[ix].end(),
            -1
        );
    }
    bool priorVertexImplicitOutEdges;
    VertexDescriptor priorVertex;
    try {
        priorVertex = priorVertex_(vertex);
        priorVertexImplicitOutEdges = implicitOutEdges_[priorVertex];
    } catch (NoVertexException) {
        priorVertexImplicitOutEdges = false;
    }
    VerticesMap::iterator iter = vertices_.find(vertex);
    if (iter == vertices_.end()) {
        return Lattice::InOutEdgesIterator(
            (layerTagManager_.canBeAppliedToImplicitSymbol(mask)
             && priorVertexImplicitOutEdges) ?
                priorVertex : -1
        );
    }
    Graph::vertex_descriptor boost_vertex = (*iter).second;
    return Lattice::InOutEdgesIterator(
        graph_[boost_vertex].inEdgesIndex[ix].begin(),
        graph_[boost_vertex].inEdgesIndex[ix].end(),
        (layerTagManager_.canBeAppliedToImplicitSymbol(mask)
         && priorVertexImplicitOutEdges) ?
            priorVertex : -1
    );
}

Lattice::InOutEdgesIterator Lattice::allOutEdges(
    Lattice::VertexDescriptor vertex
) {
    if (isLooseVertex(vertex)) {
        return Lattice::InOutEdgesIterator(
            boost::out_edges(vertices_[vertex], graph_),
            -1
        );
    }
    VerticesMap::iterator iter = vertices_.find(vertex);
    if (iter == vertices_.end()) {
        return Lattice::InOutEdgesIterator(
            implicitOutEdges_[vertex] ? vertex : -1
        );
    }
    return Lattice::InOutEdgesIterator(
        boost::out_edges(iter->second, graph_),
        implicitOutEdges_[vertex] ? vertex : -1
    );
}

Lattice::InOutEdgesIterator Lattice::allInEdges(
    Lattice::VertexDescriptor vertex
) {
    if (isLooseVertex(vertex)) {
        return Lattice::InOutEdgesIterator(
            boost::in_edges(vertices_[vertex], graph_),
            -1
        );
    }
    bool priorVertexImplicitOutEdges;
    VertexDescriptor priorVertex;
    try {
        priorVertex = priorVertex_(vertex);
        priorVertexImplicitOutEdges = implicitOutEdges_[priorVertex];
    } catch (NoVertexException) {
        priorVertexImplicitOutEdges = false;
    }
    VerticesMap::iterator iter = vertices_.find(vertex);
    if (iter == vertices_.end()) {
        return Lattice::InOutEdgesIterator(
            priorVertexImplicitOutEdges ? priorVertex : -1
        );
    }
    return Lattice::InOutEdgesIterator(
        boost::in_edges(iter->second, graph_),
        priorVertexImplicitOutEdges ? priorVertex : -1
    );
}

Lattice::EdgeDescriptor Lattice::firstOutEdge(
    Lattice::VertexDescriptor vertex,
    LayerTagMask mask
) {
    InOutEdgesIterator oe(outEdges(vertex, mask));
    if (oe.hasNext()) return oe.next();
    std::stringstream errorSs;
    if (isLooseVertex(vertex)) {
        errorSs << "No out-edges found for loose vertex " << getLooseVertexIndex(vertex) << ".";
    } else {
        errorSs << "No out-edges found for vertex " << getVertexRawCharIndex(vertex) << ".";
    }
    throw NoEdgeException(errorSs.str());
}

Lattice::EdgeDescriptor Lattice::firstInEdge(
    Lattice::VertexDescriptor vertex,
    LayerTagMask mask
) {
    if (inEdges(vertex, mask).hasNext()) return inEdges(vertex, mask).next();
    std::stringstream errorSs;
    if (isLooseVertex(vertex)) {
        errorSs << "No in-edges found for loose vertex " << getLooseVertexIndex(vertex) << ".";
    } else {
        errorSs << "No in-edges found for vertex " << getVertexRawCharIndex(vertex) << ".";
    }
    throw NoEdgeException(errorSs.str());
}

Lattice::EdgesSortedBySourceIterator Lattice::edgesSortedBySource(LayerTagMask mask) {
    return Lattice::EdgesSortedBySourceIterator(*this, mask);
}

Lattice::EdgesSortedBySourceIterator Lattice::allEdgesSortedBySource() {
    return edgesSortedBySource(layerTagManager_.anyTag());
}

Lattice::EdgesSortedByTargetIterator Lattice::edgesSortedByTarget(LayerTagMask mask) {
    return Lattice::EdgesSortedByTargetIterator(*this, mask);
}

Lattice::EdgesSortedByTargetIterator Lattice::allEdgesSortedByTarget() {
    return edgesSortedByTarget(layerTagManager_.anyTag());
}

LayerTagManager& Lattice::getLayerTagManager() {
    return layerTagManager_;
}

AnnotationItemManager& Lattice::getAnnotationItemManager() {
    return annotationItemManager_;
}

const AnnotationItem Lattice::getEdgeAnnotationItem(Lattice::EdgeDescriptor edge) {
    if (edge.isExplicit()) {
        return graph_[edge.descriptor].item;
    }
    std::string::iterator iter = allText_.begin() + edge.implicitIndex;
    std::string::iterator end = allText_.end();
    std::string symbol;
    utf8::append(utf8::next(iter, end), std::back_inserter(symbol));
    return AnnotationItem(
        "'" + symbol,
        StringFrag(allText_, edge.implicitIndex, symbolLength_(edge.implicitIndex))
    );
}

const LayerTagCollection& Lattice::getEdgeLayerTags(Lattice::EdgeDescriptor edge) const {
    if (edge.isExplicit()) {
        return graph_[edge.descriptor].tagList;
    }
    return getSymbolTag();
}

Lattice::VertexDescriptor Lattice::getEdgeSource(EdgeDescriptor edge) const {
    return VertexDescriptor(getEdgeSourceInternalIndex_(edge));
}

Lattice::VertexDescriptor Lattice::getEdgeTarget(EdgeDescriptor edge) const {
    return VertexDescriptor(getEdgeTargetInternalIndex_(edge));
}

int Lattice::getEdgeBeginIndex(Lattice::EdgeDescriptor edge) const {
    int result = getEdgeSourceInternalIndex_(edge);
    if (isLooseVertex(result)) {
        throw WrongVertexException("Edge source is loose. Loose vertices have no raw char index");
    }
    return result;
}

int Lattice::getEdgeEndIndex(Lattice::EdgeDescriptor edge) const {
    int result = getEdgeTargetInternalIndex_(edge);
    if (isLooseVertex(result)) {
        throw WrongVertexException("Edge target is loose. Loose vertices have no raw char index");
    }
    return result;
}

int Lattice::getEdgeLength(Lattice::EdgeDescriptor edge) const {
    if (edge.isExplicit()) {
        if (isLooseVertex(getEdgeSource(edge)) || isLooseVertex(getEdgeTarget(edge))) {
            throw WrongVertexException("Edges linking loose vertices have no well-defined length");
        }
        return graph_[boost::target(edge.descriptor, graph_)].index
            - graph_[boost::source(edge.descriptor, graph_)].index;
    }
    return symbolLength_(edge.implicitIndex);
}

bool Lattice::isEdgeHidden(Lattice::EdgeDescriptor edge) const {
    return !edge.isExplicit()
        && hiddenImplicitOutEdges_[edge.implicitIndex]
        && !visibleImplicitOutEdges_[edge.implicitIndex];
}

const std::list<Lattice::Partition> & Lattice::getEdgePartitions(
    Lattice::EdgeDescriptor edge
) const {
    if (edge.isExplicit()) {
        return graph_[edge.descriptor].partitions;
    }
    return emptyPartitionList_;
}

Lattice::Score Lattice::getEdgeScore(Lattice::EdgeDescriptor edge) const {
    if (edge.isExplicit()) {
        return graph_[edge.descriptor].score;
    }
    return 0.0;
}

const std::string& Lattice::getAllText() const {
    return allText_;
}

const std::string Lattice::getEdgeText(EdgeDescriptor edge) const {
    if (isLooseVertex(getEdgeSource(edge)) || isLooseVertex(getEdgeTarget(edge))) {
        return "@"; // temporarily for edges with loose vertices
    }
    return allText_.substr(getEdgeBeginIndex(edge), getEdgeLength(edge));
}

const StringFrag Lattice::getEdgeTextAsStringFrag(EdgeDescriptor edge) const {
    if (isLooseVertex(getEdgeSource(edge)) || isLooseVertex(getEdgeTarget(edge))) {
        return StringFrag("@"); // temporarily for edges with loose vertices
    }
    return StringFrag(allText_, getEdgeBeginIndex(edge), getEdgeLength(edge));
}

std::string Lattice::getSequenceText(const EdgeSequence& sequence) {
    std::string r;
    EdgeSequence::Iterator esi(*this, sequence);
    while (esi.hasNext()) {
        r += getEdgeText(esi.next());
    }
    return r;
}

StringFrag Lattice::getSequenceTextAsStringFrag(const EdgeSequence& sequence) {
    StringFrag r;
    EdgeSequence::Iterator esi(*this, sequence);
    while (esi.hasNext()) {
        r.append(getEdgeTextAsStringFrag(esi.next()));
    }
    return r;
}

std::string Lattice::getPartitionText(const Partition& partition) {
    return getSequenceText(partition.getSequence());
}

const std::string Lattice::getAnnotationText(EdgeDescriptor edge) {
    return getEdgeAnnotationItem(edge).getText();
}

const StringFrag Lattice::getAnnotationTextAsStringFrag(EdgeDescriptor edge) {
    return getEdgeAnnotationItem(edge).getTextAsStringFrag();
}

const std::string Lattice::getAnnotationCategory(EdgeDescriptor edge) {
    return getEdgeAnnotationItem(edge).getCategory();
}

std::vector<Lattice::EdgeDescriptor> Lattice::getChildren(
    EdgeDescriptor parent,
    LayerTagMask mask) {

    std::vector<EdgeDescriptor> results;

    VertexDescriptor sourceVertex = getEdgeSource(parent);
    VertexDescriptor targetVertex = getEdgeTarget(parent);

    InOutEdgesIterator iter = outEdges(sourceVertex, mask);

    while (iter.hasNext()) {
        EdgeDescriptor potentialChild = iter.next();

        VertexDescriptor itsTargetVertex = getEdgeTarget(potentialChild);

        if (itsTargetVertex == targetVertex) {
            const std::list<Partition>& partitions = getEdgePartitions(potentialChild);

            BOOST_FOREACH(Partition partition, partitions) {
                Partition::Iterator partitionIter(*this, partition);

                while (partitionIter.hasNext()) {
                    EdgeDescriptor itsParent = partitionIter.next();
                    if (itsParent == parent)
                        results.push_back(potentialChild);
                }
            }
        }
    }

    return results;
}

boost::optional<Lattice::EdgeDescriptor> Lattice::getParent(EdgeDescriptor child) {
    return getParent(child, getLayerTagManager().anyTag());
}

boost::optional<Lattice::EdgeDescriptor> Lattice::getParent(
    EdgeDescriptor child,
    LayerTagMask mask) {

    const std::list<Partition>& partitions = getEdgePartitions(child);

    BOOST_FOREACH(Partition partition, partitions) {
        Partition::Iterator partitionIter(*this, partition);

        while (partitionIter.hasNext()) {
            EdgeDescriptor parent = partitionIter.next();

            if (matches(getEdgeLayerTags(parent), mask))
                return boost::optional<EdgeDescriptor>(parent);
        }
    }

    return boost::optional<EdgeDescriptor>();
}


boost::optional<Lattice::EdgeDescriptor> Lattice::getEdgeOrigin(std::string tagName, EdgeDescriptor edge) {
    boost::optional<EdgeDescriptor> parent = getParent(edge);
    while (parent) {
        if (getLayerTagManager().isThere(tagName, getEdgeLayerTags(*parent))) {
            return boost::optional<EdgeDescriptor>(*parent);
        }
        parent = getParent(*parent);
    }
    return boost::optional<EdgeDescriptor>();
}


boost::optional<std::string> Lattice::getEdgeLemma(EdgeDescriptor edge) {
    boost::optional<EdgeDescriptor> resultEdge = getEdgeOrigin(LEMMA_TAG_NAME, edge);
    if (resultEdge) {
        return boost::optional<std::string>(getAnnotationText(*resultEdge));
    }
    return boost::optional<std::string>();
}


void Lattice::runCutter(Cutter& cutter, LayerTagMask mask, LayerTagMask superMask) {
    EdgesSortedBySourceIterator edgeIter(*this, superMask);

    while (edgeIter.hasNext()) {
        cutter.reset();
        runCutterOnEdge_(cutter, edgeIter.next(), mask);
    }
}

bool Lattice::isBlank(EdgeDescriptor edge) {
    if (
        matches(getEdgeLayerTags(edge), tokenMask_) &&
        getAnnotationCategory(edge) == "B"
    ) {
        return true;
    }
    return false;
}


Lattice::EdgeSequence Lattice::getPath(VertexDescriptor& vertex, LayerTagMask mask) {
    bool nextVertexFound = true;

    Lattice::EdgeSequence::Builder pathBuilder(*this);

    do {
        InOutEdgesIterator iter = outEdges(vertex, mask);

        if (iter.hasNext()) {
            EdgeDescriptor bestOne = iter.next();

            while (iter.hasNext())
                bestOne = iter.next();

            pathBuilder.addEdge(bestOne);
            vertex = getEdgeTarget(bestOne);
        } else
            nextVertexFound = false;

    } while (nextVertexFound);

    return pathBuilder.build();
}

Lattice::EdgeSequence Lattice::getPathSkippingBlanks(
    VertexDescriptor& vertex,
    LayerTagMask mask
) {
    return getPathWithSkipping(vertex, mask, IsBlank());
}

bool Lattice::isLooseVertex(Lattice::VertexDescriptor vd) const {
    return vd < 0;
}

int Lattice::getLooseVertexIndex(VertexDescriptor vd) const {
    if (vd > -1) {
        throw WrongVertexException("Cannot get loose vertex index for a non-loose vertex");
    }
    return -1-vd;
}

void Lattice::correctionInsert(VertexDescriptor here, std::string text) {
    VertexDescriptor from = here;
    VertexDescriptor to;
    std::string::iterator iter = text.begin();
    std::string::iterator end = text.end();
    while (iter != end) {
        to = addLooseVertex();
        std::string symbol;
        utf8::append(utf8::next(iter, end), std::back_inserter(symbol));
        addEdge(
            from,
            to,
            AnnotationItem("'"+symbol, StringFrag(symbol)),
            getSymbolTag()
        );
        from = to;
    }
    try {
        EdgeDescriptor nextEdge
            = firstOutEdge(here, symbolMask_);
        addEdge(to, getEdgeTarget(nextEdge), getEdgeAnnotationItem(nextEdge), getSymbolTag());
    } catch (NoEdgeException) {
    }
}

void Lattice::correctionErase(VertexDescriptor from, VertexDescriptor to) {
    if (from != to) {
        try {
            EdgeDescriptor nextEdge
                = firstOutEdge(to, symbolMask_);
            addEdge(
                from,
                getEdgeTarget(nextEdge),
                getEdgeAnnotationItem(nextEdge),
                getSymbolTag()
            );
        } catch (NoEdgeException) {
            try {
                EdgeDescriptor prevEdge
                    = firstOutEdge(to, symbolMask_);
                addEdge(
                    getEdgeSource(prevEdge),
                    to,
                    getEdgeAnnotationItem(prevEdge),
                    getSymbolTag()
                );
            } catch (NoEdgeException) {
                throw WrongVertexException("Cannot erase the selected text range.");
            }
        }
    }
}

void Lattice::correctionReplace(VertexDescriptor from, VertexDescriptor to, std::string text) {
    try {
        std::string::iterator iter = text.begin();
        std::string::iterator end = text.end();
        while (iter != end) {
            if (from == to) {
                return correctionInsert(from, text);
            }
            std::string symbol;
            utf8::append(utf8::next(iter, end), std::back_inserter(symbol));
            EdgeDescriptor edge
                = firstOutEdge(from, symbolMask_);
            VertexDescriptor vertex = getEdgeTarget(edge);
            if (symbol != getAnnotationText(edge)) {
                addEdge(
                    from,
                    vertex,
                    AnnotationItem("'"+symbol, StringFrag(symbol)),
                    getSymbolTag()
                );
            }
            from = vertex;
        }
        correctionErase(from, to);
    } catch (NoEdgeException) {
        throw WrongVertexException("Cannot replace the selected text range.");
    }
}

int Lattice::countEdges(VertexDescriptor from, VertexDescriptor to) {
    return edgeCounterHash_[std::pair<VertexDescriptor, VertexDescriptor>(from, to)];
}

int Lattice::countAllVertices() {
    return 1 + utf8::distance(allText_.begin(), allText_.end()) + nLooseVertices_;
}


int Lattice::addTagMaskIndex_(LayerTagMask tagMask) {
    TagMasksHash::iterator li = indexedTagMasks_.find(tagMask);
    if (li != indexedTagMasks_.end()) {
        return li->second;
    } else {
        int ix = indexedTagMasks_.size();
        BOOST_FOREACH(VerticesMap::value_type iv, vertices_) {
            graph_[iv.second].outEdgesIndex.push_back(
                std::list<EdgeDescriptorWrapperToFoolBoost146OrGnu461>()
            );
            BOOST_FOREACH(Graph::edge_descriptor edge, boost::out_edges(iv.second, graph_)) {
                if (matches(graph_[edge].tagList, tagMask)) {
                    graph_[iv.second].outEdgesIndex[ix].push_back(edge);
                }
            }
            graph_[iv.second].inEdgesIndex.push_back(
                std::list<EdgeDescriptorWrapperToFoolBoost146OrGnu461>()
            );
            BOOST_FOREACH(Graph::edge_descriptor edge, boost::in_edges(iv.second, graph_)) {
                if (matches(graph_[edge].tagList, tagMask)) {
                    graph_[iv.second].inEdgesIndex[ix].push_back(edge);
                }
            }
        }
        indexedTagMasks_.insert(std::pair<LayerTagMask, int>(tagMask, ix));
        return ix;
    }
}

void Lattice::resizeImplicitEdgesStructures_() {
    implicitOutEdges_.resize(allText_.length()+1);
    hiddenImplicitOutEdges_.resize(allText_.length()+1);
    visibleImplicitOutEdges_.resize(allText_.length()+1);
}

Lattice::VertexDescriptor Lattice::priorVertex_(Lattice::VertexDescriptor vertex) {
    if (vertex == 0) {
        throw NoVertexException("Beginning vertex has no prior vertex.");
    }
    if (vertex < 0) {
        throw NoVertexException("Loose vertices have no prior vertex.");
    }
    std::string::iterator begin = allText_.begin();
    std::string::iterator iter = begin + vertex;
    std::string symbol;
    utf8::append(utf8::prior(iter, begin), std::back_inserter(symbol));
    return vertex - symbol.length();
}

size_t Lattice::symbolLength_(int ix) const {
    return symbolLength(allText_, ix);
}

const LayerTagCollection& Lattice::getSymbolTag() const {
    return symbolTag_;
}

const LayerTagMask& Lattice::getSymbolMask() const {
    return symbolMask_;
}

int Lattice::getEdgeSourceInternalIndex_(Lattice::EdgeDescriptor edge) const {
    if (edge.isExplicit()) {
        return graph_[boost::source(edge.descriptor, graph_)].index;
    }
    return edge.implicitIndex;
}

int Lattice::getEdgeTargetInternalIndex_(Lattice::EdgeDescriptor edge) const {
    if (edge.isExplicit()) {
        return graph_[boost::target(edge.descriptor, graph_)].index;
    }
    return edge.implicitIndex + symbolLength_(edge.implicitIndex);
}


Lattice::EdgeUsage::EdgeUsage(EdgeDescriptor edge)
    :edge_(edge), role_(NULL_ZVALUE) {
}


Lattice::EdgeUsage::EdgeUsage(EdgeDescriptor edge, zvalue role)
    :edge_(edge), role_(role) {
}

Lattice::EdgeDescriptor Lattice::EdgeUsage::getEdge() const {
    return edge_;
}

zvalue Lattice::EdgeUsage::getRole() const {
    return role_;
}

Lattice::EdgeSequence::EdgeSequence() : begin(0), end(0) {
}

Lattice::EdgeSequence::Iterator::Iterator(
    Lattice & lattice,
    const EdgeSequence & edgeSequence
) :
    lattice_(lattice),
    edgeSequence_(edgeSequence),
    ei_(edgeSequence.links.begin()),
    si_(edgeSequence.begin)
{ }

bool Lattice::EdgeSequence::Iterator::hasNext() {
    if (edgeSequence_.links.empty()) {
        return si_ < edgeSequence_.end;
    } else {
        return ei_ != edgeSequence_.links.end();
    }
}

Lattice::EdgeDescriptor Lattice::EdgeSequence::Iterator::next() {
    return nextUsage().getEdge();
}

Lattice::EdgeUsage Lattice::EdgeSequence::Iterator::nextUsage() {
    if (edgeSequence_.links.empty()) {
        if (si_ >= edgeSequence_.end) {
            throw NoEdgeException("EdgeSequence::Iterator has no next edges.");
        }
        int currentSymbol = si_;
        si_ += lattice_.symbolLength_(si_);
        return Lattice::EdgeUsage(
            lattice_.firstOutEdge(
                lattice_.getVertexForRawCharIndex(currentSymbol),
                lattice_.getSymbolMask()));
    } else {
        if (ei_ == edgeSequence_.links.end()) {
            throw NoEdgeException("EdgeSequence::Iterator has no next edges.");
        }
        return *(ei_++);
    }
}

Lattice::EdgeDescriptor Lattice::EdgeSequence::firstEdge(Lattice & lattice) const {
    if (links.empty()) {
        return lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(begin),
            lattice.getSymbolMask()
        );
    } else {
        return links.front().getEdge();
    }
}

Lattice::EdgeDescriptor Lattice::EdgeSequence::lastEdge(Lattice & lattice) const {
    if (links.empty()) {
        return lattice.firstInEdge(
            lattice.getVertexForRawCharIndex(end),
            lattice.getSymbolMask()
        );
    } else {
        return links.back().getEdge();
    }
}

Lattice::EdgeDescriptor Lattice::EdgeSequence::nthEdge(Lattice & lattice, size_t index) const {
    if (links.empty()) {
        const char* latticeText = lattice.getAllText().c_str();

        const char* begIter = latticeText + begin;
        const char* endIter = latticeText + end;
        const char* foundIter = begIter;
        utf8::advance(foundIter, index, endIter);

        return lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(foundIter - latticeText),
            lattice.getSymbolMask()
        );
    } else {
        return links[index].getEdge();
    }
}

zvalue Lattice::EdgeSequence::nthRole(Lattice & /*lattice*/, size_t index) const {
    if (links.empty()) {
        return NULL_ZVALUE;
    } else {
        return links[index].getRole();
    }
}

LayerTagCollection Lattice::EdgeSequence::gatherPlaneTags(Lattice& lattice) const {
    std::vector<EdgeUsage>::const_iterator iter = links.begin();

    if (iter != links.end()) {

        LayerTagCollection tags = lattice.getLayerTagManager().onlyPlaneTags(
            lattice.getEdgeLayerTags(iter->getEdge()));

        ++iter;

        while (iter != links.end()) {
            LayerTagCollection linkPlaneTags =
                lattice.getLayerTagManager().onlyPlaneTags(
                    lattice.getEdgeLayerTags(iter->getEdge()));

            tags = createUnion(tags, linkPlaneTags);

            ++iter;
        }

        return tags;
    }

    return lattice.getLayerTagManager().createEmptyTagCollection();
}

bool Lattice::EdgeSequence::empty() const {
    return links.empty() && begin >= end;
}

size_t Lattice::EdgeSequence::size(Lattice & lattice) const {
    if (links.empty()) {
        return utf8::distance(
            lattice.getAllText().begin() + begin,
            lattice.getAllText().begin() + end
        );
    } else {
        return links.size();
    }
}

Lattice::EdgeSequence::Builder& Lattice::EdgeSequence::Builder::addEdge(
    EdgeDescriptor edge, zvalue role) {
    if (begin <= end && lattice_.isEdgeHidden(edge)) {
        if (links.empty()) {
            begin = lattice_.getEdgeBeginIndex(edge);
            end = lattice_.getEdgeEndIndex(edge);
        } else if (lattice_.getEdgeBeginIndex(edge) == end) {
            end = lattice_.getEdgeEndIndex(edge);
        } else {
            end = 0;
        }
    } else {
        begin = 1;
        end = 0;
    }

    links.push_back(EdgeUsage(edge, role));

    return *this;
}

Lattice::EdgeSequence Lattice::EdgeSequence::Builder::build() {
    if (begin < end) {
        return Lattice::EdgeSequence(begin, end);
    } else {
        return Lattice::EdgeSequence(links);
    }
}

Lattice::EdgeSequence::EdgeSequence(const std::vector<EdgeDescriptor>& aLinks) :
    begin(0),
    end(0)
{
    links.reserve(aLinks.size());
    BOOST_FOREACH(EdgeDescriptor edge, aLinks) {
        links.push_back(EdgeUsage(edge));
    }
}

Lattice::EdgeSequence::EdgeSequence(const std::vector<EdgeUsage>& aLinks) :
    links(aLinks),
    begin(0),
    end(0)
{ }


Lattice::EdgeSequence::EdgeSequence(int aBegin, int aEnd) :
    begin(aBegin),
    end(aEnd)
{ }


Lattice::Partition::Partition(
    LayerTagCollection aTagList,
    EdgeSequence aSequence,
    Score aScore,
    int aRuleId,
    int aTreeChoice
) :
    sequence_(aSequence),
    tagList_(aTagList),
    score_(aScore),
    ruleId_(aRuleId),
    treeChoice_(aTreeChoice)
{ }


const Lattice::EdgeSequence& Lattice::Partition::getSequence() const {
    return sequence_;
}

const LayerTagCollection& Lattice::Partition::getTagList() const {
    return tagList_;
}

const Lattice::Score& Lattice::Partition::getScore() const {
    return score_;
}

const int& Lattice::Partition::getRuleId() const {
    return ruleId_;
}

const int& Lattice::Partition::getTreeChoice() const {
    return treeChoice_;
}

Lattice::VertexDescriptor Lattice::firstSequenceVertex_(const EdgeSequence& sequence) {
    return getEdgeSource(sequence.firstEdge(*this));
}

Lattice::VertexDescriptor Lattice::lastSequenceVertex_(const EdgeSequence& sequence) {
    return getEdgeTarget(sequence.lastEdge(*this));
}

void Lattice::runCutterOnEdge_(Cutter& cutter, EdgeDescriptor edge, LayerTagMask mask) {

    VertexDescriptor vertex = getEdgeSource(edge);

    EdgeSequence sequence = getPath(vertex, mask);

    if (sequence.empty())
        return;

    StringFrag text = getSequenceTextAsStringFrag(sequence);

    size_t pos = 0;

    EdgeSequence::Iterator sequenceIter(*this, sequence);

    LayerTagCollection tags = layerTagManager_.createTagCollection(cutter.layerTags());

    do {
        size_t prevPos = pos;

        AnnotationItem item = cutter.cutOff(text, pos);

        int itemLength = (pos == std::string::npos ? text.length() : pos) - prevPos;

        EdgeSequence itemSequence = cutSequenceByTextLength_(sequenceIter, itemLength);

        addEdge(firstSequenceVertex_(itemSequence),
                lastSequenceVertex_(itemSequence),
                item,
                tags,
                itemSequence);

    } while (pos < text.length() && pos != std::string::npos);
}

Lattice::EdgeSequence Lattice::cutSequenceByTextLength_(
    EdgeSequence::Iterator& sequenceIterator,
    int length
) {
    int lengthGathered = 0;

    EdgeSequence::Builder sequenceBuilder(*this);

    while (sequenceIterator.hasNext() && lengthGathered < length) {
        EdgeDescriptor ed = sequenceIterator.next();
        sequenceBuilder.addEdge(ed);
        lengthGathered += getEdgeText(ed).length();
    }

    return sequenceBuilder.build();
}

inline bool compareSecond_(
    std::pair<int, int> p1,
    std::pair<int, int> p2
) {
    return p1.second < p2.second;
}

Lattice::VertexIterator::VertexIterator(Lattice& lattice, bool skipUselessVertices) :
    lattice_(lattice),
    vd_(0),
    withLooseVertices_(lattice.nLooseVertices_ > 0),
    skipUselessVertices_(skipUselessVertices)
{
    if (withLooseVertices_) {
        std::list<Graph::vertex_descriptor> vertexContainer;
        boost::topological_sort(lattice_.graph_, std::front_inserter(vertexContainer));
        BOOST_FOREACH(Graph::vertex_descriptor vd, vertexContainer) {
            iterContainer_.push_back(
                std::pair<Lattice::VertexDescriptor, int>(lattice.graph_[vd].index, f_(vd))
            );
        }
        VertexDescriptor vd = 0;
        while (vd < int(lattice.allText_.length())) {
            if (
                lattice.vertices_.find(vd) == lattice.vertices_.end()
                && !shouldBeSkipped_(vd)
            ) {
                iterContainer_.push_back(std::pair<Lattice::VertexDescriptor, int>(vd, vd));
            }
            vd += lattice_.symbolLength_((int)vd);
        }
        if (lattice.vertices_.find(vd) == lattice.vertices_.end()) {
            iterContainer_.push_back(std::pair<Lattice::VertexDescriptor, int>(vd, vd));
        }
        std::stable_sort(iterContainer_.begin(), iterContainer_.end(), compareSecond_);
        ici_ = iterContainer_.begin();
    }
}

bool Lattice::VertexIterator::hasNext() {
    if (withLooseVertices_) {
        return ici_ != iterContainer_.end();
    }
    while (vd_ <= (int)lattice_.allText_.length()) {
        if (shouldBeSkipped_(vd_)) {
            nextRealVertex_();
        } else {
            return true;
        }
    }
    return false;
}

Lattice::VertexDescriptor Lattice::VertexIterator::next() {
    if (withLooseVertices_) {
        if (ici_ != iterContainer_.end()) return (*(ici_++)).first;
    }
    while (vd_ <= (int)lattice_.allText_.length()) {
        if (shouldBeSkipped_(vd_)) {
            nextRealVertex_();
        } else {
            Lattice::VertexDescriptor returnedVertexDescriptor = vd_;
            nextRealVertex_();
            return returnedVertexDescriptor;
        }
    }
    throw NoVertexException("Vertex iterator has no next vertex.");
}

void Lattice::VertexIterator::nextRealVertex_() {
    if (vd_ < (int)lattice_.allText_.length())
        vd_ += lattice_.symbolLength_((int)vd_);
    else
        ++vd_;
}

bool Lattice::VertexIterator::shouldBeSkipped_(Lattice::VertexDescriptor vd) {
    return skipUselessVertices_
        && lattice_.vertices_.find(vd) == lattice_.vertices_.end()
        && !lattice_.implicitOutEdges_[vd]
        && (vd == 0 || !lattice_.implicitOutEdges_[lattice_.priorVertex_(vd)]);
}

int Lattice::VertexIterator::f_(Graph::vertex_descriptor vd) {
    int index = lattice_.graph_[vd].index;
    if (index > -1) {
        return index;
    }
    int maxIndex = -1;
    BOOST_FOREACH(Graph::edge_descriptor edge, boost::in_edges(vd, lattice_.graph_)) {
        index = f_(boost::source(edge, lattice_.graph_));
        if (index > maxIndex) {
            maxIndex = index;
        }
    }
    return maxIndex;
}

bool Lattice::InOutEdgesIterator::hasNext() {
    if (implicitIndex_ > -1) return true;
    switch (type_) {
    case EDGE_DESCRIPTOR_ITER : return edi_ != ediEnd_;
    case OUT_EDGE_ITER : return oei_ != oeiEnd_;
    case IN_EDGE_ITER : return iei_ != ieiEnd_;
    case IMPLICIT_ITER : return implicitIndex_ > -1;
    }

    throw NoEdgeException("unexpected state");
}

Lattice::EdgeDescriptor Lattice::InOutEdgesIterator::next() {
    if (implicitIndex_ > -1) {
        int index = implicitIndex_;
        implicitIndex_ = -1;
        return EdgeDescriptor(index);
    }
    switch (type_) {
    case EDGE_DESCRIPTOR_ITER :
        if (edi_ != ediEnd_) return EdgeDescriptor(*(edi_++));
        break;
    case OUT_EDGE_ITER :
        if (oei_ != oeiEnd_) return EdgeDescriptor(*(oei_++));
        break;
    case IN_EDGE_ITER :
        if (iei_ != ieiEnd_) return EdgeDescriptor(*(iei_++));
        break;
    case IMPLICIT_ITER :
        break;
    }
    throw NoEdgeException("InOutEdgesIterator has no next edges.");
}

Lattice::SortedEdgesIterator::SortedEdgesIterator(
    Lattice& lattice,
    LayerTagMask mask
) :
    lattice_(lattice),
    mask_(mask),
    vi_(lattice)
{ }

bool Lattice::SortedEdgesIterator::hasNext() {
    if (ei_.hasNext()) return true;
    while (vi_.hasNext()) {
        ei_ = getEdgesIterator_(vi_.next());
        if (ei_.hasNext()) return true;
    }
    return false;
}

Lattice::EdgeDescriptor Lattice::SortedEdgesIterator::next() {
    if (ei_.hasNext()) return ei_.next();
    while (vi_.hasNext()) {
        ei_ = getEdgesIterator_(vi_.next());
        if (ei_.hasNext()) return ei_.next();
    }
    throw NoEdgeException("SortedEdgesIterator has no next edges.");
}

Lattice::SortedEdgesIterator::~SortedEdgesIterator() {
}

Lattice::EdgesSortedBySourceIterator::EdgesSortedBySourceIterator(
    Lattice& lattice,
    LayerTagMask mask
) : SortedEdgesIterator(lattice, mask) {
    ei_ = lattice_.outEdges(0, mask_);
    if (vi_.hasNext()) {
        ei_ = getEdgesIterator_(vi_.next());
    }
}

Lattice::InOutEdgesIterator Lattice::EdgesSortedBySourceIterator::getEdgesIterator_(
    Lattice::VertexDescriptor vd
) {
    return lattice_.outEdges(vd, mask_);
}

Lattice::EdgesSortedByTargetIterator::EdgesSortedByTargetIterator(
    Lattice& lattice,
    LayerTagMask mask
) : SortedEdgesIterator(lattice, mask) {
    ei_ = lattice_.inEdges(0, mask_);
    if (vi_.hasNext()) {
        ei_ = getEdgesIterator_(vi_.next());
    }
}

Lattice::InOutEdgesIterator Lattice::EdgesSortedByTargetIterator::getEdgesIterator_(
    Lattice::VertexDescriptor vd
) {
    return lattice_.inEdges(vd, mask_);
}
