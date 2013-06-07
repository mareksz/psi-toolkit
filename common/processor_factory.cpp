#include "processor_factory.hpp"
#include "processor_file_fetcher.hpp"
#include "logging.hpp"

#include <sstream>
#include <fstream>

Processor* ProcessorFactory::createProcessor(const boost::program_options::variables_map& options) {
    return doCreateProcessor(options);
}

boost::program_options::options_description ProcessorFactory::optionsHandled() {
    return doOptionsHandled();
}

std::string ProcessorFactory::getContinuation(
    const boost::program_options::variables_map& options) const {

    return doGetContinuation(options);
}

double ProcessorFactory::getQualityScore(
    const boost::program_options::variables_map& options) const {

    return doGetQualityScore(options);
}

double ProcessorFactory::getEstimatedTime(
    const boost::program_options::variables_map& options) const {

    return doGetEstimatedTime(options);
}

double ProcessorFactory::doGetQualityScore(
    const boost::program_options::variables_map& /*options*/) const {

    return 0;
}

double ProcessorFactory::doGetEstimatedTime(
    const boost::program_options::variables_map& /*options*/) const {

    return -1.0;
}

boost::filesystem::path ProcessorFactory::getFile() {
    return doGetFile();
}

boost::filesystem::path ProcessorFactory::getDataDirectory() const {
    return getItsData(doGetFile());
}

std::string ProcessorFactory::getName() const {
    return doGetName();
}

std::list<std::string> ProcessorFactory::getAliases() {
    return doGetAliases();
}

std::string ProcessorFactory::getDescription() {
    return doGetDescription();
}

std::string ProcessorFactory::doGetDescription() {
    return getDataFile("description.txt");
}

std::string ProcessorFactory::getDetailedDescription() {
    return doGetDetailedDescription();
}

std::string ProcessorFactory::doGetDetailedDescription() {
    return getDataFile("detailed_description.txt", false);
}

std::string ProcessorFactory::getType() const {
    return doGetType();
}

std::string ProcessorFactory::getSubType() {
    return doGetSubType();
}

bool ProcessorFactory::isSubTyped() const {
    return doIsSubTyped();
}

std::list<std::string> ProcessorFactory::doGetAliases() {
    return std::list<std::string>();
}

ProcessorFactory::~ProcessorFactory() {
}

bool ProcessorFactory::checkRequirements(const boost::program_options::variables_map& options,
                                         std::ostream & message) const {
    return doCheckRequirements(options, message);
}

bool ProcessorFactory::doCheckRequirements(const boost::program_options::variables_map& /*options*/,
                                           std::ostream & /*message*/) const {
    return true;
}

std::string ProcessorFactory::getDataFile(std::string fileName, bool printWarning /* = true */) {
    std::string content("");

    ProcessorFileFetcher fileFetcher(doGetFile());
    try {
        boost::filesystem::path pathToDataFile = fileFetcher.getOneFile("%ITSDATA%/" + fileName);
        content = getFileContent(pathToDataFile);
    }
    catch (FileFetcher::Exception& err) {
        if (printWarning) {
            WARN("An error occured when trying to open the " << fileName << " file: " << ": "
                << err.what());
        }
    }

    return content;
}

std::string ProcessorFactory::getFileContent(boost::filesystem::path path) {
    std::stringstream content;
    content << std::ifstream( path.string().c_str() ).rdbuf();

    return content.str();
}

const std::string ProcessorFactory::OPTION_LABEL = "Allowed options";
