#ifndef UNUMSUNT_HDR
#define UNUMSUNT_HDR


#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>

#include "annotator.hpp"
#include "language_dependent_annotator_factory.hpp"
#include "lang_specific_processor_file_fetcher.hpp"


class Unumsunt : public Annotator {

public:

    class Factory : public LanguageDependentAnnotatorFactory {
        virtual Annotator* doCreateAnnotator(
            const boost::program_options::variables_map& options);

        virtual void doAddLanguageIndependentOptionsHandled(
            boost::program_options::options_description& optionsDescription);

        virtual std::string doGetName() const;
        virtual boost::filesystem::path doGetFile() const;

        virtual std::list<std::list<std::string> > doRequiredLayerTags();

        virtual std::list<std::list<std::string> > doOptionalLayerTags();

        virtual std::list<std::string> doProvidedLayerTags();

        static const std::string DEFAULT_RULE_FILE;
    };

    Unumsunt(std::string rulesPath);

private:

    class Worker : public LatticeWorker {
    public:
        Worker(Unumsunt& processor, Lattice& lattice);
    private:
        virtual void doRun();
        Unumsunt& processor_;
    };

    virtual LatticeWorker* doCreateLatticeWorker(Lattice& lattice);

    virtual std::string doInfo();

    std::string rulesPath_;

};


#endif
