#include "joiner.hpp"

Joiner::Joiner(
    const std::string& langCode,
    const std::string& leftMaskSpecification,
    const std::string& rightMaskSpecification,
    const std::string& outTagsSpecification,
    bool takeLeftText,
    bool takeLeftCategory,
    HandlingAttributes handlingAttributes,
    bool outerJoin)
    :langCode_(langCode),
     leftMaskSpecification_(
         LayerTagManager::multiplyMaskListByLangCode(
             LayerTagManager::splitMaskSpecification(leftMaskSpecification),
             langCode)),
     rightMaskSpecification_(
         LayerTagManager::multiplyMaskListByLangCode(
             LayerTagManager::splitMaskSpecification(rightMaskSpecification),
             langCode)),
     outTags_(
         LayerTagManager::splitCollectionSpecification(outTagsSpecification)),
     takeLeftText_(takeLeftText),
     takeLeftCategory_(takeLeftCategory),
     handlingAttributes_(handlingAttributes),
     outerJoin_(outerJoin) {
}

Annotator* Joiner::Factory::doCreateAnnotator(
    const boost::program_options::variables_map& options) {

    std::string lang = options["lang"].as<std::string>();

    std::string leftMaskSpecification =
        options["left-mask"].as<std::string>();

    std::string rightMaskSpecification =
        options["right-mask"].as<std::string>();

    std::string outTags =
        options["out-tags"].as<std::string>();

    bool takeLeftText = !options.count("take-right-text");
    bool takeLeftCategory = !options.count("take-right-category");
    HandlingAttributes handlingAttributes =
        options.count("take-left-attributes") ? TAKE_LEFT_ATTRIBUTES
        : (options.count("take-right-attributes") ? TAKE_RIGHT_ATTRIBUTES
           : MERGE_ATTRIBUTES);
    bool outerJoin = !options.count("no-outer-join");

    return new Joiner(
        lang,
        leftMaskSpecification,
        rightMaskSpecification,
        outTags,
        takeLeftText,
        takeLeftCategory,
        handlingAttributes,
        outerJoin);
}

std::list<std::list<std::string> > Joiner::Factory::doRequiredLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::list<std::string> > Joiner::Factory::doRequiredLayerTags(
    const boost::program_options::variables_map& options) {

    std::list<std::list<std::string> > leftTags =
        LayerTagManager::splitMaskSpecification(
            options["left-mask"].as<std::string>());

    std::list<std::list<std::string> > rightTags =
        LayerTagManager::splitMaskSpecification(
            options["left-mask"].as<std::string>());


    std::list<std::list<std::string> > tags;

    for(std::list<std::list<std::string> >::iterator leftIter
            = leftTags.begin();
        leftIter != leftTags.end();
        ++leftIter) {
        for(std::list<std::list<std::string> >::iterator rightIter
                = rightTags.begin();
            rightIter != rightTags.end();
            ++rightIter) {

            std::list<std::string> tagsConj = *leftIter;
            std::list<std::string>& rightList = *rightIter;

            tagsConj.insert(tagsConj.end(), rightList.begin(), rightList.end());

            tags.push_back(tagsConj);
        }
    }

    return tags;
}

std::list<std::list<std::string> > Joiner::Factory::doOptionalLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::string> Joiner::Factory::doProvidedLayerTags() {
    return std::list<std::string>();
}

std::list<std::string> Joiner::Factory::doProvidedLayerTags(
    const boost::program_options::variables_map& options) {

    return LayerTagManager::splitCollectionSpecification(
        options["out-tags"].as<std::string>());
}

void Joiner::Factory::doAddLanguageIndependentOptionsHandled(
    boost::program_options::options_description& optionsDescription) {

    optionsDescription.add_options()
        ("left-mask", boost::program_options::value<std::string>()
         ->required(),
         "tag mask specification for the \"left\" mask")
        ("right-mask", boost::program_options::value<std::string>()
         ->required(),
         "tag mask specification for the \"right\" mask")
        ("out-tags", boost::program_options::value<std::string>()
         ->required(),
         "tags for generated edges")
        ("take-right-text", "take the text field from the \"right\" edge (by default, the left one is used)")
        ("take-right-category", "take the text field from the \"right\" edge (by default, the right one is used")
        ("take-left-attributes", "take only attributes from the \"left\" edge (by default the attributes from the \"left\" one and the \"right\" one are merged")
        ("take-right-attributes", "take only attributes from the \"right\" edge")
        ("no-outer-join", "switch off \"outer\" join");
}

boost::filesystem::path Joiner::Factory::doGetFile() const {
    return __FILE__;
}

std::string Joiner::Factory::doGetName() const {
    return "joiner";
}


LatticeWorker* Joiner::doCreateLatticeWorker(Lattice & lattice) {
    return new Worker(*this, lattice);
}


Joiner::Worker::Worker(Joiner & processor, Lattice & lattice) :
    LatticeWorker(lattice), processor_(processor)
{ }


void Joiner::Worker::doRun() {
    LayerTagManager& tagManager = lattice_.getLayerTagManager();
    LayerTagMask leftMask = tagManager.getAlternativeMaskFromTagNames(
        processor_.leftMaskSpecification_);
    LayerTagMask rightMask = tagManager.getAlternativeMaskFromTagNames(
        processor_.rightMaskSpecification_);

    LayerTagCollection providedTags = tagManager.createTagCollection(
        processor_.outTags_);

    Lattice::EdgesSortedBySourceIterator leftIter(lattice_, leftMask);

    while (leftIter.hasNext()) {
        Lattice::EdgeDescriptor edge = leftIter.next();

        boost::optional<Lattice::EdgeDescriptor> parent = lattice_.getParent(edge);

        bool foundRightEdge = false;

        if (parent) {
            std::vector<Lattice::EdgeDescriptor> rightEdges =
                lattice_.getChildren(*parent, rightMask);

            BOOST_FOREACH(Lattice::EdgeDescriptor rightEdge, rightEdges) {
                foundRightEdge = true;

                std::string category =
                    lattice_.getAnnotationCategory(
                        processor_.takeLeftCategory_ ? edge : rightEdge);
                std::string text =
                    lattice_.getAnnotationText(
                        processor_.takeLeftText_ ? edge : rightEdge);

                AnnotationItem attrsSource
                    = lattice_.getEdgeAnnotationItem(
                        (processor_.handlingAttributes_ ==
                         TAKE_RIGHT_ATTRIBUTES)
                        ? rightEdge
                        : edge);

                AnnotationItem annotationItem(
                    attrsSource, category, StringFrag(text));

                if (processor_.handlingAttributes_ ==
                    MERGE_ATTRIBUTES) {
                    lattice_.getAnnotationItemManager().addValues(
                        annotationItem,
                        lattice_.getEdgeAnnotationItem(rightEdge));
                }

                Lattice::EdgeSequence::Builder builder(lattice_);

                builder.addEdge(edge);
                builder.addEdge(rightEdge);

                Lattice::VertexDescriptor fromVertex = lattice_.getEdgeSource(edge);
                Lattice::VertexDescriptor toVertex = lattice_.getEdgeTarget(edge);

                lattice_.addEdge(
                    fromVertex,
                    toVertex,
                    annotationItem,
                    providedTags,
                    builder.build());
            }
        }

        if (!foundRightEdge && processor_.outerJoin_) {

            lattice_.addPartitionToEdge(
                edge,
                providedTags);
        }
    }
}

std::string Joiner::doInfo() {
    return "joiner";
}
