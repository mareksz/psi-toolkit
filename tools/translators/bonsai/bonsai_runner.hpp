#pragma once

#include "lattice.hpp"
#include "annotator_factory.hpp"

#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "bonsai.hpp"

class BonsaiRunner {

public:
    BonsaiRunner(const boost::program_options::variables_map& options);

    static std::string getName();
    static std::string getSubType();
    static boost::filesystem::path getFile();

    static AnnotatorFactory::LanguagesHandling languagesHandling(
        const boost::program_options::variables_map& options);

    static std::list<std::string> languagesHandled(
        const boost::program_options::variables_map& options);

    static boost::program_options::options_description optionsHandled();
    static std::list<std::string> providedLayerTags();
    static std::list<std::list<std::string> > requiredLayerTags();
    static std::string getContinuation(
        const boost::program_options::variables_map& options);

    std::list<std::string> tagsToOperateOn();

    void processEdge(Lattice& lattice, Lattice::EdgeDescriptor edge);

    class Exception : public PsiException  {
    public:
        Exception(const std::string& msg): PsiException(msg) {
        }

        virtual ~Exception() throw() {}
    };

private:
    static std::list<std::string> mainLayerTags_();
    static std::list<std::string> coreLayerTags_();
    void createTags_(const std::string& trg_lang);
    
    std::vector<std::string>
    normalizePaths_(boost::filesystem::path&, const std::vector<std::string> &);
    
    std::string langCode_;
    std::list<std::string> tags_;
    std::list<std::string> targetFormTags_;
    std::list<std::string> targetTokenTags_;
    
    boost::filesystem::path configFile_;
    poleng::bonsai::TranslatorPtr translator_;
    
    static const std::string DEFAULT_CONFIG_FILE;
};
