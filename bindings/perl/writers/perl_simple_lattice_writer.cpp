#include "perl_simple_lattice_writer.hpp"

#if HAVE_PERL_BINDINGS

#include "logging.hpp"

std::string PerlSimpleLatticeWriter::getFormatName() {
    return "simple";
}

LatticeWriter<Sink> * PerlSimpleLatticeWriter::Factory::doCreateLatticeWriter(
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

    return new PerlSimpleLatticeWriter(
        options.count("linear"),
        options.count("no-alts"),
        options.count("with-blank"),
        quoter.unescape(options["tags"].as<std::string>()),
        higherOrderTags,
        options.count("with-args"),
        fallbackTags
        );
}

boost::program_options::options_description PerlSimpleLatticeWriter::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ("fallback-tags",
            boost::program_options::value< std::vector<std::string> >()->multitoken(),
            "tags that should be printed out if basic tags not found")
        ("linear",
            "skips cross-edges")
        ("no-alts",
            "skips alternative edges")
        ("with-blank",
            "does not skip edges with whitespace text")
        ("tags",
            boost::program_options::value<std::string>()->default_value("token"),
            "basic tag or tags separated by commas (conjunction) or semicolons (alternative)")
        ("spec",
            boost::program_options::value< std::vector<std::string> >()->multitoken(),
            "specification of higher-order tags")
        ("with-args",
            "if set, then returns text with annotation as a hash element")
        ;

    return optionsDescription;
}


std::string PerlSimpleLatticeWriter::Factory::doGetName() const {
    return "perl-simple-writer";
}

boost::filesystem::path PerlSimpleLatticeWriter::Factory::doGetFile() const {
    return __FILE__;
}

std::string PerlSimpleLatticeWriter::doInfo() {
    return "Perl simple writer";
}

PerlSimpleLatticeWriter::Worker::Worker(PerlSimpleLatticeWriter & processor,
                                 Sink & output,
                                 Lattice& lattice):
    WriterWorker<Sink>(output, lattice),
    processor_(processor) {
}

void PerlSimpleLatticeWriter::Worker::doRun() {

    PerlSimpleLatticeWriterOutputIterator outputIterator(
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

PerlSimpleLatticeWriter::Worker::~Worker() {
}

#endif
