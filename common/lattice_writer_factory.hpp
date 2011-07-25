#ifndef LATTICE_WRITER_FACTORY_HDR
#define LATTICE_WRITER_FACTORY_HDR

#include "processor_factory.hpp"
#include "lattice_writer.hpp"

class LatticeWriterFactory : public ProcessorFactory {

public:
    /**
     * Creates a lattice writer with the options specified.
     */
    LatticeWriter* createLatticeWriter(boost::program_options::variables_map options);

private:

    virtual LatticeWriter* doCreateProcessor(boost::program_options::variables_map options);

    virtual LatticeWriter* doCreateLatticeWriter(boost::program_options::variables_map options) = 0;

};

#endif
