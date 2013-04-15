#include "json_lattice_writer.hpp"

#include "logging.hpp"

std::string JSONLatticeWriter::getFormatName() {
    return "simple";
}

LatticeWriter<std::ostream> * JSONLatticeWriter::Factory::doCreateLatticeWriter(
    const boost::program_options::variables_map& options
) {
    PsiQuoter quoter;

    std::set<std::string> higherOrderTags;
    if (options.count("spec")) {
        std::vector<std::string> spec = options["spec"].as< std::vector<std::string> >();
        std::vector<std::string>::iterator si = spec.begin();
        while (si != spec.end()) {
            std::string tag = quoter.unescape(*si);
            higherOrderTags.insert(tag);

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

    return new JSONLatticeWriter(
        options.count("linear"),
        options.count("no-alts"),
        options.count("with-blank"),
        quoter.unescape(options["tags"].as<std::string>()),
        higherOrderTags,
        options.count("with-args"),
        fallbackTags
        );
}

boost::program_options::options_description JSONLatticeWriter::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription(OPTION_LABEL);

    optionsDescription.add_options()
        ("fallback-tags",
            boost::program_options::value< std::vector<std::string> >()->multitoken(),
            "tags that should be printed out if basic tags not found")
        ("linear",
            "skip cross-edges")
        ("no-alts",
            "skip alternative edges")
        ("with-blank",
            "do not skip edges with whitespace text")
        ("tags",
            boost::program_options::value<std::string>()->default_value("token"),
            "basic tag or tags separated by commas (conjunction) or semicolons (alternative)")
        ("spec",
            boost::program_options::value< std::vector<std::string> >()->multitoken(),
            "specification of higher-order tags")
        ("with-args",
            "if set, returns text with annotation as a hash element");

    return optionsDescription;
}


std::string JSONLatticeWriter::Factory::doGetName() const {
    return "json-simple-writer";
}

boost::filesystem::path JSONLatticeWriter::Factory::doGetFile() const {
    return __FILE__;
}

std::string JSONLatticeWriter::doInfo() {
    return "JSON simple writer";
}

JSONLatticeWriter::Worker::Worker(JSONLatticeWriter & processor,
                                  std::ostream & output,
                                 Lattice& lattice):
    WriterWorker<std::ostream>(output, lattice),
    processor_(processor) {
}

void JSONLatticeWriter::Worker::doRun() {

    JSONLatticeWriterOutputIterator outputIterator(
                                                         getOutputStream(),
                                                         processor_.isWithArgs()
        );

    std::vector<std::string> handledTags;
    BOOST_FOREACH(std::string higherOrderTag, processor_.getHigherOrderTags()) {
        handledTags.push_back(higherOrderTag);
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
}

JSONLatticeWriter::Worker::~Worker() {
}
