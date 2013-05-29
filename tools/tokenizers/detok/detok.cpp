#include "detok.hpp"

Annotator* Detok::Factory::doCreateAnnotator(
    const boost::program_options::variables_map& options) {

    std::string lang = options["lang"].as<std::string>();
    LangSpecificProcessorFileFetcher fileFetcher(__FILE__, lang);

    Detok *detok = new Detok(lang);

    return detok;
}

void Detok::Factory::doAddLanguageIndependentOptionsHandled(
    boost::program_options::options_description& /*optionsDescription*/) {
}

std::string Detok::Factory::doGetName() const {
    return "detok";
}

boost::filesystem::path Detok::Factory::doGetFile() const {
    return __FILE__;
}

std::list<std::list<std::string> > Detok::Factory::doRequiredLayerTags() {
    return boost::assign::list_of(
        boost::assign::list_of(std::string("token")));
}

std::list<std::list<std::string> > Detok::Factory::doOptionalLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::string> Detok::Factory::doProvidedLayerTags() {
    std::list<std::string> layerTags;
    layerTags.push_back("ttoken");
    return layerTags;
}

LatticeWorker* Detok::doCreateLatticeWorker(Lattice& lattice) {
    return new Worker(*this, lattice);
}

Detok::Worker::Worker(Detok& processor, Lattice& lattice):
    LatticeWorker(lattice), processor_(processor) {
}

void Detok::Worker::doRun() {
    INFO("detokenizer working");

    std::string langCode = processor_.langCode_;

    LayerTagMask mask = lattice_.getLayerTagManager().getMaskWithLangCode(
        "token", langCode);

    Lattice::EdgesSortedByTargetIterator iter(lattice_, mask);

    std::map<int, Lattice::EdgeDescriptor> orderMap;

    while (iter.hasNext()) {
        Lattice::EdgeDescriptor edge = iter.next();

        AnnotationItem item = lattice_.getEdgeAnnotationItem(edge);

        int position = ZVALUE_TO_INTEGER(
            lattice_.getAnnotationItemManager().getValue(
                item,
                "SurfacePosition"));

        orderMap[position] = edge;
    }

    std::string finalText;

    bool firstToken = true;

    for (std::map<int, Lattice::EdgeDescriptor>::const_iterator orderMapIter
             = orderMap.begin();
         orderMapIter != orderMap.end();
         ++orderMapIter) {

        Lattice::EdgeDescriptor edge = (*orderMapIter).second;

        std::string tokenText = lattice_.getAnnotationText(edge);

        INFO("JOINING TOKEN: " << tokenText);

        if (firstToken)
            firstToken = false;
        else if (shouldBePrecededBySpace_(tokenText)
                 && shouldBeFollowedBySpace_(finalText))
            finalText += " ";

        finalText += tokenText;
    }

    Lattice::VertexDescriptor fromVertex = lattice_.addLooseVertex();
    Lattice::VertexDescriptor toVertex = lattice_.addLooseVertex();

    INFO("TEXT: " << finalText);

    AnnotationItem textItem("TEXT", StringFrag(finalText));

    lattice_.addEdge(
        fromVertex,
        toVertex,
        textItem,
        lattice_.getLayerTagManager().createSingletonTagCollectionWithLangCode(
            "text", langCode));
}

bool Detok::Worker::shouldBePrecededBySpace_(const std::string& text) {
    if (text.empty())
        return false;

    switch(text[0]) {
    case '.':
    case ',':
    case ';':
    case ':':
    case ')':
    case ']':
    case '}':
    case '!':
    case '?':
        return false;
    }

    return true;
}

bool Detok::Worker::shouldBeFollowedBySpace_(const std::string& text) {
    if (text.empty())
        return false;

    switch(text[text.length()-1]) {
    case '(':
    case '[':
    case '{':
        return false;
    }

    return true;
}


std::string Detok::doInfo() {
    return "detokenizer";
}

Detok::Detok(const std::string& langCode)
    :langCode_(langCode) {
}
