#ifndef SELECTOR_HDR
#define SELECTOR_HDR

#include <list>
#include <string>

#include <boost/program_options/options_description.hpp>

#include "language_dependent_annotator_factory.hpp"

class Selector: public Annotator
{

public:

    class Factory: public LanguageDependentAnnotatorFactory
    {
    private:
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

        virtual std::string doGetContinuation(
            const boost::program_options::variables_map& options) const;

        virtual boost::filesystem::path doGetFile() const;

        virtual std::string doGetName() const;
        virtual std::string doGetSubType() const;

        static const std::string DEFAULT_IN_TAG;
        static const std::string DEFAULT_OUT_TAGS;
    };

    Selector(
        const std::string& inTag,
        const std::string& testTag,
        const std::string& outTagsSpecification);

private:
    class Worker : public LatticeWorker
    {
    public:
        Worker(Selector& processor, Lattice& lattice);
    private:
        virtual void doRun();
        Selector& processor_;
        LayerTagCollection outTags_;
    };

    virtual LatticeWorker* doCreateLatticeWorker(Lattice& lattice);

    virtual std::string doInfo();

    std::string inTag_;
    std::string testTag_;
    std::list<std::string> outTags_;
};

#endif
