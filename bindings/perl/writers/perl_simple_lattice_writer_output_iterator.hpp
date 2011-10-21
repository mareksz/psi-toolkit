#ifndef PERL_SIMPLE_LATTICE_WRITER_OUTPUT_ITERATOR_HDR
#define PERL_SIMPLE_LATTICE_WRITER_OUTPUT_ITERATOR_HDR

#include <string>

#include "lattice_writer_output_iterator.hpp"
#include "perl_lattice_writer_output.hpp"

/**
 * Writes simple lattice writer's output to the stream.
 */
class PerlSimpleLatticeWriterOutputIterator : public LatticeWriterOutputIterator {
public:
    PerlSimpleLatticeWriterOutputIterator(
        PerlLatticeWriterOutput & output
    );

    virtual ~PerlSimpleLatticeWriterOutputIterator();

private:
    PerlLatticeWriterOutput & output_;

    virtual void doPutElement(const std::string& element);

    virtual void doOpenAlternative();

    virtual void doCloseAlternative();

    virtual void doOpenGroup(const std::string& group);

    virtual void doCloseGroup(const std::string& group);

    virtual void doFlush();
};

#endif
