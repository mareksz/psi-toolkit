#ifndef SELECTOR_HDR
#define SELECTOR_HDR

#include <boost/program_options/options_description.hpp>

#include "language_dependent_annotator_factory.hpp"

class Selector: public Annotator
{

public:

    class Factory: public LanguageDependentAnnotatorFactory
    {
    public:
        virtual Annotator* doCreateAnnotator(const boost::program_options::variables_map& options);

        virtual std::list<std::list<std::string> > doRequiredLayerTags();

        virtual std::list<std::list<std::string> > doRequiredLayerTags(
            const boost::program_options::variables_map& options);

        virtual std::list<std::list<std::string> > doOptionalLayerTags();

        virtual std::list<std::string> doProvidedLayerTags();

        virtual void doAddLanguageIndependentOptionsHandled(
            boost::program_options::options_description& optionsDescription);

        virtual AnnotatorFactory::LanguagesHandling doLanguagesHandling(
            const boost::program_options::variables_map& /* options */) const;

        virtual std::list<std::string> doProvidedLayerTags(
            const boost::program_options::variables_map& options);

        virtual boost::filesystem::path doGetFile() const;

        virtual std::string doGetName() const;
        virtual std::string doGetSubType() const;
    };

    Selector(
        const std::string& inputTag,
        const std::string& testTag,
        const std::string& outputTag);

private:
    class Worker : public LatticeWorker
    {
    public:
        Worker(Selector& processor, Lattice& lattice);
    private:
        virtual void doRun();
        Selector& processor_;
        LayerTagCollection outputTags_;
    };

    virtual LatticeWorker* doCreateLatticeWorker(Lattice& lattice);

    virtual std::string doInfo();

    std::string inputTag_;
    std::string testTag_;
    std::string outputTag_;
};

#endif
