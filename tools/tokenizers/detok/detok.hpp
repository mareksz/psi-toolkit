//#pragma once

#include "config.hpp"
#include "annotator.hpp"
#include "language_dependent_annotator_factory.hpp"
#include "lang_specific_processor_file_fetcher.hpp"

class Detok : public Annotator {

public:

    class Factory : public LanguageDependentAnnotatorFactory {
        virtual Annotator* doCreateAnnotator(
            const boost::program_options::variables_map& options);

        virtual void doAddLanguageIndependentOptionsHandled(
            boost::program_options::options_description& optionsDescription);

        virtual std::string doGetName() const;
        virtual std::string doGetSubType() const;
        virtual boost::filesystem::path doGetFile() const;

        virtual std::list<std::list<std::string> > doRequiredLayerTags();

        virtual std::list<std::list<std::string> > doOptionalLayerTags();

        virtual std::list<std::string> doProvidedLayerTags();

        virtual AnnotatorFactory::LanguagesHandling doLanguagesHandling(
            const boost::program_options::variables_map& /*options*/) const {

            return AnnotatorFactory::LANGUAGE_INDEPENDENT;
        }

    };

    Detok(const std::string& langCode);

    private:
    class Worker : public LatticeWorker {
        public:
            Worker(Detok& processor, Lattice& lattice);
        private:
            bool shouldBePrecededBySpace_(const std::string& text);
            bool shouldBeFollowedBySpace_(const std::string& text);

            virtual void doRun();
            Detok& processor_;
        };

    virtual LatticeWorker* doCreateLatticeWorker(Lattice& lattice);

    virtual std::string doInfo();

    std::string langCode_;
};
