#include "aligning_writer_worker.hpp"

AligningWriterWorker::AligningWriterWorker(std::ostream& outputStream, Lattice& lattice)
    :WriterWorker<std::ostream>(outputStream, lattice), currentPos_(0) {
}

AligningWriterWorker::~AligningWriterWorker() {
}

unsigned int AligningWriterWorker::alignOutput_(std::string output) {
    getOutputStream() << output;
    currentPos_ += utf8::distance(output.begin(), output.end());
    return currentPos_;
}

unsigned int AligningWriterWorker::alignOutput_(
    std::string output, unsigned int pos, char padChar) {

    getOutputStream() << output;
    currentPos_ += utf8::distance(output.begin(), output.end());
    for (; currentPos_ < pos; ++currentPos_) {
        getOutputStream() << padChar;
    }
    return currentPos_;
}

unsigned int AligningWriterWorker::alignOutputNewline_() {
    getOutputStream() << std::endl;
    currentPos_ = 0;
    return currentPos_;
}

void AligningWriterWorker::setAlignments_(std::vector<unsigned int> alignments) {
    alignments_ = alignments;
}

void AligningWriterWorker::printTableRow_(std::vector<std::string> cellsContents) {
    if (!cellsContents.empty()) {
        for (size_t i = 0; i < cellsContents.size() - 1; ++i) {
            alignOutput_(cellsContents[i], getAlignment_(i));
            alignOutput_(" ");
        }
        alignOutput_(cellsContents.back());
    }
    alignOutputNewline_();
}

unsigned int AligningWriterWorker::getColumnWidth_(size_t columnNum) {
    return getAlignment_(columnNum) - ((columnNum > 0) ? getAlignment_(columnNum - 1) : 0);
}

unsigned int AligningWriterWorker::getAlignment_(size_t columnNum) {
    if (columnNum < alignments_.size()) {
        return alignments_[columnNum];
    }
    unsigned int lastAlignment = 0;
    if (!alignments_.empty()) {
        lastAlignment = alignments_.back();
    }
    return lastAlignment + DEFAULT_COLUMN_WIDTH * (columnNum - alignments_.size());
}

const unsigned int AligningWriterWorker::DEFAULT_COLUMN_WIDTH = 20;
