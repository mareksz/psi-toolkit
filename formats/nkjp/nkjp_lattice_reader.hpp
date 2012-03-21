#ifndef NKJP_LATTICE_READER_HDR
#define NKJP_LATTICE_READER_HDR


#include "stream_lattice_reader.hpp"
#include "lattice_reader_factory.hpp"

class NKJPLatticeReader : public StreamLatticeReader {

public:

    enum AnnotationLayer {
        TEXT,
        ANN_SEGMENTATION,
        ANN_MORPHOSYNTAX,
        ANN_SENSES,
        ANN_WORDS,
        ANN_NAMED,
        ANN_GROUPS
    };

    virtual std::string getFormatName();

    class Factory : public LatticeReaderFactory<std::istream> {
    public:
        virtual ~Factory();

    private:
        virtual LatticeReader<std::istream>* doCreateLatticeReader(
            const boost::program_options::variables_map& options);

        virtual boost::program_options::options_description doOptionsHandled();

        virtual std::string doGetName();
        virtual boost::filesystem::path doGetFile();
    };

    NKJPLatticeReader(
        std::string layer
    ) {
        if (layer == "text") {
            layer_ = TEXT;
        } else if (layer == "segmentation") {
            layer_ = ANN_SEGMENTATION;
        } else if (layer == "morphosyntax") {
            layer_ = ANN_MORPHOSYNTAX;
        } else if (layer == "senses") {
            layer_ = ANN_SENSES;
        } else if (layer == "words") {
            layer_ = ANN_WORDS;
        } else if (layer == "named") {
            layer_ = ANN_NAMED;
        } else if (layer == "groups") {
            layer_ = ANN_GROUPS;
        } else {
            throw PsiException(
                "Unknown type of NKJP layer: " + layer + ". Use one of the following: " +
                "text segmentation morphosyntax senses words named groups"
            );
        }
    }

    AnnotationLayer getLayer() { return layer_; }

private:
    virtual std::string doInfo();

    class Worker : public ReaderWorker<std::istream> {
    public:
        Worker(NKJPLatticeReader& processor,
               std::istream& inputStream,
               Lattice& lattice);

        virtual void doRun();

    private:
        Processor& processor_;
        LayerTagCollection textTags_;

        void appendSegmentToLattice_(std::string segment);
    };

    virtual ReaderWorker<std::istream>* doCreateReaderWorker(
        std::istream& inputStream, Lattice& lattice) {

        return new Worker(*this, inputStream, lattice);
    }

    AnnotationLayer layer_;
};

#endif
