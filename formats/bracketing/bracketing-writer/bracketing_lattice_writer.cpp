#include "bracketing_lattice_writer.hpp"


#include <iterator>
#include <utility>

#include "bracketing_quoter.hpp"


std::string BracketingLatticeWriter::getFormatName() {
    return "bracketing";
}


LatticeWriter<std::ostream>* BracketingLatticeWriter::Factory::doCreateLatticeWriter(
    const boost::program_options::variables_map& options
) {
    BracketingQuoter quoter;

    std::vector<std::string> showOnlyTags;
    std::vector<std::string> filter;
    std::vector<std::string> showAttributes;

    if (options.count("show-only-tags")) {
        showOnlyTags = options["show-only-tags"].as< std::vector<std::string> >();
    } else if (options.count("filter")) {
        showOnlyTags = options["filter"].as< std::vector<std::string> >();
    }

    if (options.count("filter")) {
        filter = options["filter"].as< std::vector<std::string> >();
    }
    if (options.count("show-attributes")) {
        showAttributes = options["show-attributes"].as< std::vector<std::string> >();
    }

    return new BracketingLatticeWriter(
        quoter.unescape(options["opening-bracket"].as<std::string>()),
        quoter.unescape(options["closing-bracket"].as<std::string>()),
        quoter.unescape(options["tag-separator"].as<std::string>()),
        showOnlyTags,
        filter,
        quoter.unescape(options["av-pairs-separator"].as<std::string>()),
        quoter.unescape(options["av-separator"].as<std::string>()),
        showAttributes,
        !options.count("skip-symbol-edges"),
        !options.count("with-blank"),
        !options.count("no-collapse"),
        options.count("disambig")
    );
}


boost::program_options::options_description BracketingLatticeWriter::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription(OPTION_LABEL);

    optionsDescription.add_options()
        ("opening-bracket",
            boost::program_options::value<std::string>()->default_value("["),
            "the actual format of opening brackets")
        ("closing-bracket",
            boost::program_options::value<std::string>()->default_value("]"),
            "the actual format of closing brackets")
        ("tag-separator",
            boost::program_options::value<std::string>()->default_value(","),
            "separates tags")
        ("show-only-tags",
            boost::program_options::value< std::vector<std::string> >()->multitoken(),
            "limits the tag names that will appear in `%T` substitions")
        ("filter",
            boost::program_options::value< std::vector<std::string> >()->multitoken(),
            "filters the edges by tags")
        ("av-pairs-separator",
            boost::program_options::value<std::string>()->default_value(","),
            "separates the attribute-value pairs")
        ("av-separator",
            boost::program_options::value<std::string>()->default_value("="),
            "separates the attribute and its value")
        ("show-attributes",
            boost::program_options::value< std::vector<std::string> >()->multitoken(),
            "the attributes to be shown")
        ("skip-symbol-edges",
            "skip symbol edges")
        ("with-blank",
            "do not skip edges with whitespace text")
        ("no-collapse",
            "do not collapse duplicate edge labels")
        ("disambig",
            "choose only one partition");

    return optionsDescription;
}


std::string BracketingLatticeWriter::Factory::doGetName() const {
    return "bracketing-writer";
}


boost::filesystem::path BracketingLatticeWriter::Factory::doGetFile() const {
    return __FILE__;
}


BracketingLatticeWriter::BracketingLatticeWriter(
    std::string openingBracket,
    std::string closingBracket,
    std::string tagSeparator,
    std::vector<std::string> showOnlyTags,
    std::vector<std::string> filter,
    std::string avPairsSeparator,
    std::string avSeparator,
    std::vector<std::string> showAttributes,
    bool showSymbolEdges,
    bool skipBlank,
    bool collapseDuplicate,
    bool disambig
) :
    openingBracket_(openingBracket),
    closingBracket_(closingBracket),
    tagSeparator_(tagSeparator),
    showOnlyTags_(showOnlyTags.begin(), showOnlyTags.end()),
    filter_(filter.begin(), filter.end()),
    avPairsSeparator_(avPairsSeparator),
    avSeparator_(avSeparator),
    showAttributes_(showAttributes.begin(), showAttributes.end()),
    showSymbolEdges_(showSymbolEdges),
    skipBlank_(skipBlank),
    collapseDuplicate_(collapseDuplicate),
    disambig_(disambig)
{ }


bool BracketingLatticeWriter::areSomeInFilter(std::list<std::string> & tags) {
    if (filter_.empty()) return true;
    BOOST_FOREACH(std::string tag, tags) {
        if (filter_.count(tag)) return true;
    }
    return false;
}


std::set<std::string> BracketingLatticeWriter::intersectOnlyTags(
    std::set<std::string> tags
) {
    if (showOnlyTags_.empty()) {
        return tags;
    }
    std::set<std::string> result;
    set_intersection(
        showOnlyTags_.begin(), showOnlyTags_.end(),
        tags.begin(), tags.end(),
        std::inserter(result, result.begin())
    );
    return result;
}


std::map<std::string, std::string> BracketingLatticeWriter::filterAttributes(
    std::map<std::string, std::string> avMap
) {
    if (showAttributes_.empty()) {
        return avMap;
    }
    std::map<std::string, std::string> result;
    typedef std::pair<std::string, std::string> StrStrPair;
    BOOST_FOREACH(StrStrPair avPair, avMap) {
        if (showAttributes_.count(avPair.first)) {
            result.insert(avPair);
        }
    }
    return result;
}


std::string BracketingLatticeWriter::doInfo() {
    return "Bracketing writer";
}


BracketingLatticeWriter::Worker::Worker(BracketingLatticeWriter& processor,
                                 std::ostream& outputStream,
                                 Lattice& lattice):
    AligningWriterWorker(outputStream, lattice), processor_(processor) {
}


void BracketingLatticeWriter::Worker::doRun() {

    if (processor_.isCollapseDuplicate()) {
        doRun_< std::set<EdgeData>, std::set<EdgePrintData> >();
    } else {
        doRun_< std::vector<EdgeData>, std::vector<EdgePrintData> >();
    }

}


BracketingLatticeWriter::Worker::~Worker() {
}


EdgeData BracketingLatticeWriter::Worker::getEdgeData_(Lattice::EdgeDescriptor edge) {
    AnnotationItem annotationItem = lattice_.getEdgeAnnotationItem(edge);
    std::set<std::string> tags;
    std::list<std::string> tagsList
        = lattice_.getLayerTagManager().getTagNames(lattice_.getEdgeLayerTags(edge));
    tags.insert(tagsList.begin(), tagsList.end());
    std::map<std::string, std::string> avMap;
    avMap = lattice_.getAnnotationItemManager().getAVMap(annotationItem);
    return EdgeData(
        lattice_,
        edge,
        processor_.intersectOnlyTags(tags),
        annotationItem.getCategory(),
        annotationItem.getText(),
        processor_.filterAttributes(avMap),
        lattice_.getEdgeScore(edge)
    );
}


bool BracketingLatticeWriter::Worker::shouldBeSkipped_(Lattice::EdgeDescriptor edge) {
    if (lattice_.isDiscarded(edge)) {
        return true;
    }
    if (processor_.isSkipBlank() && lattice_.isBlank(edge)) {
        return true;
    }
    std::list<std::string> tagNames = lattice_.getLayerTagManager().getTagNames(
        lattice_.getEdgeLayerTags(edge));
    if (
        tagNames.size() == 1 &&
        tagNames.front() == Lattice::SYMBOL_TAG_NAME &&
        !processor_.isShowSymbolEdges()
    ) {
        return true;
    }
    if (!processor_.areSomeInFilter(tagNames)) {
        return true;
    }
    return false;
}
