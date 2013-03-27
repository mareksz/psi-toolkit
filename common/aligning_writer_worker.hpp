#ifndef ALIGNING_WRITER_WORKER
#define ALIGNING_WRITER_WORKER

#include <string>
#include <vector>

#include "writer_worker.hpp"

class AligningWriterWorker : public WriterWorker<std::ostream> {

public:
    AligningWriterWorker(std::ostream& outputStream, Lattice& lattice);

    virtual ~AligningWriterWorker();

protected:
    /**
     * Prints output to outputStream_ and updates cursor position.
     * Returns the new cursor position.
     */
    unsigned int alignOutput_(std::string output);

    /**
     * Prints output to outputStream_ and aligns cursor to the required position (pos) if possible.
     * Strings are aligned left.
     * Returns the real new cursor position.
     */
    unsigned int alignOutput_(std::string output, unsigned int pos, char padChar = ' ');

    /**
     * Prints newline to outputStream_ and resets cursor position.
     * Returns the new cursor position (zero).
     */
    unsigned int alignOutputNewline_();

    /**
     * Sets position of columns.
     */
    void setAlignments_(std::vector<unsigned int> alignments);

    /**
     * Prints cellsContents to outputStream_ aligning them.
     */
    void printTableRow_(std::vector<std::string> cellsContents);

private:
    unsigned int currentPos_;

    /**
     * Positions of columns.
     */
    std::vector<unsigned int> alignments_;

    /**
     * Returns position of ith column.
     */
    unsigned int getAlignment_(size_t i);

    static const unsigned int DEFAULT_COLUMN_WIDTH;

};

#endif
