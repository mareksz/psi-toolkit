#pragma once

#include "annotator_factory.hpp"
#include "language_dependent_annotator_factory.hpp"

class Joiner: public Annotator {

public:

    class Factory: public LanguageDependentAnnotatorFactory {
    public:
        virtual Annotator* doCreateAnnotator(const boost::program_options::variables_map& options);

        virtual std::list<std::list<std::string> > doRequiredLayerTags();

        virtual std::list<std::list<std::string> > doRequiredLayerTags(
            const boost::program_options::variables_map& options);

        virtual std::list<std::list<std::string> > doOptionalLayerTags();

        virtual std::list<std::string> doProvidedLayerTags();

        virtual std::list<std::string> doProvidedLayerTags(
            const boost::program_options::variables_map& options);

        virtual void doAddLanguageIndependentOptionsHandled(
            boost::program_options::options_description& optionsDescription);

        virtual boost::filesystem::path doGetFile() const;

        virtual std::string doGetName() const;
    };

    enum HandlingAttributes {
        MERGE_ATTRIBUTES,
        TAKE_LEFT_ATTRIBUTES,
        TAKE_RIGHT_ATTRIBUTES
    };

    Joiner(
        const std::string& langCode,
        const std::string& leftMaskSpecification,
        const std::string& rightMaskSpecification,
        const std::string& outTags,
        bool takeLeftText = true,
        bool takeLeftCategory = true,
        HandlingAttributes handlingAttributes = MERGE_ATTRIBUTES,
        bool outerJoin = true);

private:
    class Worker : public LatticeWorker {
    public:
        Worker(Joiner& processor, Lattice& lattice);
    private:
        virtual void doRun();
        Joiner& processor_;
    };

    virtual LatticeWorker* doCreateLatticeWorker(Lattice& lattice);

    virtual std::string doInfo();

    std::string langCode_;
    std::list<std::list<std::string> > leftMaskSpecification_;
    std::list<std::list<std::string> > rightMaskSpecification_;
    std::list<std::string> outTags_;
    bool takeLeftText_;
    bool takeLeftCategory_;
    HandlingAttributes handlingAttributes_;
    bool outerJoin_;
};
