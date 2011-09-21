#include "psi_lattice_reader.hpp"


std::string PsiLatticeReader::getFormatName() {
    return "Psi";
}

std::string PsiLatticeReader::doInfo() {
    return "Psi reader";
}

PsiLatticeReader::Worker::Worker(PsiLatticeReader& processor,
                                 std::istream& inputStream,
                                 Lattice& lattice):
    ReaderWorker(inputStream, lattice), processor_(processor) {
}

void PsiLatticeReader::Worker::doRun() {
    PsiQuoter quoter;
    PsiLRGrammar grammar;
    PsiLRAVGrammar avGrammar;
    std::string line;
    while (std::getline(inputStream_, line)) {
        PsiLRItem item;
        std::string::const_iterator begin = line.begin();
        std::string::const_iterator end = line.end();
        if (parse(begin, end, grammar, item)) {

            item.unescape(quoter);

std::stringstream tagsSs;
BOOST_FOREACH(std::string tag, item.tags) {
if (!tagsSs.str().empty()) {
tagsSs << "+";
}
tagsSs << tag;
}

DEBUG("+|" << ((item.ordinal < 10)?" ":"") << item.ordinal
    // << "|" << (item.beginningLoose?"@":" ")
    // << "|" << (item.lengthPoint?"*":" ") << (item.lengthLoose?"@":" ")
    // << "|" << tagsSs.str()
    // << "|" << item.annotationText
    << "|" << item.annotationItem.category << "|\t"
    << "|" << item.annotationItem.score << "|\t"
    << "|" << item.annotationItem.avVector << "|\t"
    << "|" << item.annotationItem.partition
    << "|");

            std::vector<std::string> avItem;
            std::string::const_iterator avBegin = item.annotationItem.avVector.begin();
            std::string::const_iterator avEnd = item.annotationItem.avVector.end();
            if (parse(avBegin, avEnd, avGrammar, avItem)) {

BOOST_FOREACH(std::string av, avItem) {
DEBUG("AV OK " << av);
}

            } else {

DEBUG("AV FAIL " << item.annotationItem.avVector);

            }

        } else {
if (!line.empty() && line[0] != '#')
DEBUG("-|" << line.substr(0,2) << "|--------");

        }
    }
}

