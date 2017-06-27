#ifndef SIMPLENORM_NORMALIZER_HDR
#define SIMPLENORM_NORMALIZER_HDR

#include <list>
#include <map>
#include <string>

#include <boost/filesystem/path.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "annotator.hpp"
#include "lattice.hpp"
#include "language_dependent_annotator_factory.hpp"


class SimplenormNormalizer : public Annotator
{

public:
    class Factory : public LanguageDependentAnnotatorFactory
    {
    private:
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

        static const std::string DEFAULT_RULE_FILE_SPEC;
    };

    SimplenormNormalizer(const std::string& lang,
                         boost::filesystem::path rules);

    void normalize(Lattice & lattice);

private:
    class Worker : public LatticeWorker
    {
    public:
        Worker(SimplenormNormalizer& processor, Lattice& lattice);
    private:
        virtual void doRun();
        SimplenormNormalizer& processor_;
    };

    virtual LatticeWorker* doCreateLatticeWorker(Lattice& lattice);

    virtual std::string doInfo();

    std::string langCode_;

    std::map<std::string, std::string> normalization_map_;
};


#endif
