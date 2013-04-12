#ifndef MAPPER_HDR
#define MAPPER_HDR

#include "lattice.hpp"
#include "lexicon_base.hpp"
#include "annotator_factory.hpp"

#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

class Mapper {

public:
    Mapper(const boost::program_options::variables_map& options);

    static std::string getName();
    static boost::filesystem::path getFile();

    static AnnotatorFactory::LanguagesHandling languagesHandling(
        const boost::program_options::variables_map& options);

    static std::list<std::string> languagesHandled(
        const boost::program_options::variables_map& options);

    static boost::program_options::options_description optionsHandled();

    static std::list<std::string> providedLayerTags();
    static std::list<std::list<std::string> > requiredLayerTags();

    static std::list<std::string> providedLayerTags(
        const boost::program_options::variables_map& options);
    static std::list<std::list<std::string> > requiredLayerTags(
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
    void readPlainText_(const boost::filesystem::path& plainTextLexicon);
    void saveBinary_(const boost::filesystem::path& binaryLexiconPath);
    void loadBinary_(const boost::filesystem::path& binaryLexiconPath);

    void addEntry_(Lattice& lattice, Lattice::EdgeDescriptor edge, const std::string& record);
    void addEntryUnknown_(Lattice& lattice, Lattice::EdgeDescriptor edge);

    void parseEntry_(const std::string& record,
                     boost::optional<std::string>& text,
                     boost::optional<std::string>& category,
                     std::vector<std::pair<std::string, std::string> >& attributes);

    void createTags_(const std::string& trg_lang);

    std::string langCode_;
    LexiconBase lexiconBase_;

    std::list<std::string> inTags_;
    std::list<std::string> outTags_;

    bool considerText_;
    bool considerCategory_;
    bool considerAttributes_;

    bool cloneText_;
    bool cloneCategory_;
    bool cloneAttributes_;

    bool addAttributes_;

    boost::optional<std::string> setText_;
    boost::optional<std::string> setCategory_;
    boost::optional<std::string> setAttributes_;
    std::vector<std::pair<std::string, std::string> > setAttributesParsed_;

    bool unknownCloneText_;
    bool unknownCloneCategory_;
    bool unknownCloneAttributes_;

    boost::optional<std::string> unknownSetText_;
    boost::optional<std::string> unknownSetCategory_;
    boost::optional<std::string> unknownSetAttributes_;
    std::vector<std::pair<std::string, std::string> > unknownSetAttributesParsed_;
};

#endif
