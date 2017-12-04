#ifndef PHRASES_LATTICE_WRITER_HDR
#define PHRASES_LATTICE_WRITER_HDR

#include "lattice_writer.hpp"
#include "lattice_writer_factory.hpp"
#include "aligning_writer_worker.hpp"


class PhrasesLatticeWriter : public LatticeWriter<std::ostream> {

public:
    virtual std::string getFormatName();

    class Factory : public LatticeWriterFactory<std::ostream> {
    private:
        virtual LatticeWriter<std::ostream>* doCreateLatticeWriter(
            const boost::program_options::variables_map& options);

        virtual boost::program_options::options_description doOptionsHandled();

        virtual std::string doGetName() const;
        virtual boost::filesystem::path doGetFile() const;
    };

    PhrasesLatticeWriter();

private:
    virtual std::string doInfo();

    class Worker : public AligningWriterWorker {
    public:
        Worker(PhrasesLatticeWriter& processor,
               std::ostream& outputStream,
               Lattice& lattice);

        virtual void doRun();

        virtual ~Worker();
    private:
        PhrasesLatticeWriter& processor_;
    };

    virtual WriterWorker<std::ostream>* doCreateWriterWorker(
        std::ostream& outputStream, Lattice& lattice) {

        return new Worker(*this, outputStream, lattice);
    }

};

#endif
