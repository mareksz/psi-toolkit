#include <boost/algorithm/string/join.hpp>

#include "console_help_formatter.hpp"

void ConsoleHelpFormatter::doFormatOneProcessorHelp(
    std::string processorName,
    std::string description,
    boost::program_options::options_description options,
    std::list<std::string> aliases,
    std::vector<TestBatch> usingExamples,
    std::list<std::string> languagesHandled,
    std::ostream& output) {

    output << "### " << processorName << " ###" << std::endl
           << std::endl
           << description << std::endl;

    if (!aliases.empty()) {
        formatAliases_(aliases, output);
        output << std::endl;
    }

    if (!languagesHandled.empty()) {
        formatLanguagesHandled_(languagesHandled, output);
        output << std::endl;
    }

    if (!usingExamples.empty())
        output << "To see one of the " << usingExamples.size()
            << " examples check installed files or visit PSI website."
            << std::endl << std::endl;

    output << options;
}

void ConsoleHelpFormatter::formatLanguagesHandled_(std::list<std::string> langCodes,
                                                   std::ostream& output) {
    output << LANGUAGES_HEADER << ": "
        << boost::algorithm::join(langCodes, ", ")
        << std::endl;
}

void ConsoleHelpFormatter::formatAliases_(std::list<std::string> aliases, std::ostream& output) {
    output << ALIASES_HEADER << ": "
        << boost::algorithm::join(aliases, ", ")
        << std::endl;
}

void ConsoleHelpFormatter::doFormatOneAlias(
    std::string aliasName,
    std::list<std::string> processorNames,
    std::ostream& output) {

    if (processorNames.empty()) return;

    output << aliasName << " -> " << boost::algorithm::join(processorNames, ", ") << std::endl;
}

void ConsoleHelpFormatter::doFormatDataFile(std::string text, std::ostream& output) {
    output << text << std::endl;
}

ConsoleHelpFormatter::~ConsoleHelpFormatter() { }
