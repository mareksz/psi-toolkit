#pragma once

#include "lattice.hpp"
#include "annotator_factory.hpp"

#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "transferer.hpp"

class TransfererRunner {

public:
    TransfererRunner(const boost::program_options::variables_map& options);

    static std::string getName();
    static boost::filesystem::path getFile();

    static AnnotatorFactory::LanguagesHandling languagesHandling(
        const boost::program_options::variables_map& options);

    static std::list<std::string> languagesHandled(
        const boost::program_options::variables_map& options);

    static boost::program_options::options_description optionsHandled();
    static std::list<std::string> providedLayerTags();
    static std::list<std::list<std::string> > requiredLayerTags();
    std::list<std::string> tagsToOperateOn();

    void processEdge(Lattice& lattice, Lattice::EdgeDescriptor edge);

    class Exception : public PsiException  {
    public:
        Exception(const std::string& msg): PsiException(msg) {
        }

        virtual ~Exception() throw() {}
    };

private:
    void createTags_(const std::string& trg_lang);

    std::string langCode_;
    std::list<std::string> tags_;

    boost::filesystem::path rulesFile_;

    static const std::string DEFAULT_RULE_FILE;
};
