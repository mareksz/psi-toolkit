#include "simple_lattice_writer.hpp"


std::string SimpleLatticeWriter::getFormatName() {
    return "simple";
}

LatticeWriter<std::ostream>* SimpleLatticeWriter::Factory::doCreateLatticeWriter(
    const boost::program_options::variables_map& options
) {
    PsiQuoter quoter;

    std::map<std::string, std::string> tagsSeparators;
    if (options.count("spec")) {
        std::vector<std::string> spec = options["spec"].as< std::vector<std::string> >();
        std::vector<std::string>::iterator si = spec.begin();
        while (si != spec.end()) {
            std::string tag = quoter.unescape(*si);
            ++si;
            if (si == spec.end()) {
                break;
            }
            tagsSeparators[tag] = quoter.unescape(*si);
            ++si;
        }
    }

    std::vector<std::string> fallbackTags;
    if (options.count("fallback-tags")) {
        std::vector<std::string> fallbackTagsEscaped
            = options["fallback-tags"].as< std::vector<std::string> >();
        BOOST_FOREACH(std::string tag, fallbackTagsEscaped) {
            fallbackTags.push_back(quoter.unescape(tag));
        }
    }

    return new SimpleLatticeWriter(
        options.count("linear"),
        options.count("no-alts"),
        options.count("with-blank"),
        quoter.unescape(options["tags"].as<std::string>()),
        quoter.unescape(options["sep"].as<std::string>()),
        quoter.unescape(options["alt-sep"].as<std::string>()),
        tagsSeparators,
        fallbackTags
    );
}

boost::program_options::options_description SimpleLatticeWriter::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription(OPTION_LABEL);

    optionsDescription.add_options()
        ("alt-sep",
            boost::program_options::value<std::string>()->default_value("|"),
            "alternative edges separator")
        ("fallback-tags",
            boost::program_options::value< std::vector<std::string> >()->multitoken(),
            "tags that should be printed out if basic tags not found")
        ("linear",
            "skip cross-edges")
        ("no-alts",
            "skip alternative edges")
        ("with-blank",
            "do not skip edges with whitespace text")
        ("sep",
            boost::program_options::value<std::string>()->default_value("\n"),
            "basic tag separator")
        ("spec",
            boost::program_options::value< std::vector<std::string> >()->multitoken(),
            "specification of higher-order tags and their separators")
        ("tags",
            boost::program_options::value<std::string>()->default_value("token"),
            "basic tag or tags separated by commas (conjunction) or semicolons (alternative)");

    return optionsDescription;
}


std::string SimpleLatticeWriter::Factory::doGetName() const {
    return "simple-writer";
}

boost::filesystem::path SimpleLatticeWriter::Factory::doGetFile() const {
    return __FILE__;
}

std::string SimpleLatticeWriter::doInfo() {
    return "Simple writer";
}

SimpleLatticeWriter::Worker::Worker(SimpleLatticeWriter& processor,
                                 std::ostream& outputStream,
                                 Lattice& lattice):
    AligningWriterWorker(outputStream, lattice), processor_(processor) {
}

void SimpleLatticeWriter::Worker::doRun() {

    SimpleLatticeWriterStreamOutputIterator outputIterator(
        getOutputStream(),
        processor_.getAltSeparator(),
        processor_.getBasicTagSeparator()
    );

    std::vector<std::string> handledTags;

    typedef std::pair<std::string, std::string> TagSeparatorPair;
    BOOST_FOREACH(TagSeparatorPair tagSeparatorPair, processor_.getTagsSeparatorsMap()) {
        outputIterator.setSeparator(tagSeparatorPair.first, tagSeparatorPair.second);
        handledTags.push_back(tagSeparatorPair.first);
    }

    LatticeIterWriter writer(
        lattice_,
        outputIterator,
        processor_.isLinear(),
        processor_.isNoAlts(),
        processor_.isWithBlank(),
        processor_.getBasicTag(),
        handledTags,
        processor_.getFallbackTags()
    );

    writer.run();

    printLine_();

}

SimpleLatticeWriter::Worker::~Worker() {
}
