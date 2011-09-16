#include "simple_lattice_writer.hpp"


std::string SimpleLatticeWriter::getFormatName() {
    return "simple";
}

LatticeWriter* SimpleLatticeWriter::Factory::doCreateLatticeWriter(
    const boost::program_options::variables_map& options
) {
    std::map<std::string, std::string> tagsSeparators;
    if (options.count("spec")) {
        std::vector<std::string> spec = options["spec"].as< std::vector<std::string> >();
        std::vector<std::string>::iterator si = spec.begin();
        while (si != spec.end()) {
            std::string tag = *si;
            ++si;
            if (si == spec.end()) {
                break;
            }
            tagsSeparators[tag] = *si;
            ++si;
        }
    }

    return new SimpleLatticeWriter(
        options.count("linear"),
        options.count("no-alts"),
        options["tag"].as<std::string>(),
        options["sep"].as<std::string>(),
        options["alt-sep"].as<std::string>(),
        tagsSeparators
    );
}

boost::program_options::options_description SimpleLatticeWriter::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ("alt-sep", boost::program_options::value<std::string>()->default_value("|"),
            "alternative edges separator")
        ("linear",
            "skips cross-edges")
        ("no-alts",
            "skips alternative edges")
        ("sep", boost::program_options::value<std::string>()->default_value("\n"),
            "basic tag separator")
        ("spec", boost::program_options::value< std::vector<std::string> >()->multitoken(),
            "specification of higher-order tags and their separators")
        ("tag", boost::program_options::value<std::string>()->default_value("token"),
            "basic tag")
        ;

    return optionsDescription;
}


std::string SimpleLatticeWriter::Factory::doGetName() {
    return "simple-writer";
}


std::string SimpleLatticeWriter::doInfo() {
    return "Simple writer";
}

SimpleLatticeWriter::Worker::Worker(SimpleLatticeWriter& processor,
                                 std::ostream& outputStream,
                                 Lattice& lattice):
    WriterWorker(outputStream, lattice), processor_(processor) {
}

void SimpleLatticeWriter::Worker::doRun() {

    SimpleLatticeWriterStreamOutputIterator outputIterator(
        getOutputStream(),
        processor_.getAltSeparator(),
        processor_.getBasicTagSeparator()
    );

    std::vector<std::string> handledTags;

    std::map<std::string, std::string> tagsSeparatorsMap = processor_.getTagsSeparatorsMap();
    for (
        std::map<std::string, std::string>::iterator mi = tagsSeparatorsMap.begin();
        mi != tagsSeparatorsMap.end();
        ++mi
    ) {
        outputIterator.setSeparator((*mi).first, (*mi).second);
        handledTags.push_back((*mi).first);
    }

    LatticeIterWriter writer(
        lattice_,
        outputIterator,
        processor_.isLinear(),
        processor_.isNoAlts(),
        processor_.getBasicTag(),
        handledTags
    );

    writer.run();

    alignOutputNewline_();

}

SimpleLatticeWriter::Worker::~Worker() {
}
