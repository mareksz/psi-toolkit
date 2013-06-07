#ifndef LAMERLEMMA_HDR
#define LAMERLEMMA_HDR

#include <string>
#include <list>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/filesystem.hpp>

#include "config.hpp"
#include "annotator.hpp"
#include "language_dependent_annotator_factory.hpp"
#include "lang_specific_processor_file_fetcher.hpp"

#include "annotation_item_manager.hpp"
#include "lemmatizer_output_iterator.hpp"
#include "dictionary.hpp"

class LamerLemma {

public:
    LamerLemma(const boost::program_options::variables_map& options);

    bool lemmatize(const std::string& token,
                   AnnotationItemManager& annotationItemManager,
                   LemmatizerOutputIterator& outputIterator);

    static std::string getName();
    static std::string getSubType();
    static boost::filesystem::path getFile();
    static std::list<std::string> getLayerTags();
    std::string getLanguage() const;

    static AnnotatorFactory::LanguagesHandling languagesHandling(
        const boost::program_options::variables_map& options);

    static std::list<std::string> languagesHandled(
        const boost::program_options::variables_map& options);

    static bool checkRequirements(
        const boost::program_options::variables_map& options,
        std::ostream & message);

    static boost::program_options::options_description optionsHandled();

    static double getQualityScore(
        const boost::program_options::variables_map& options);

private:
    Dictionary dict_;
    std::string langCode_;
    int level_;

    static const std::string DEFAULT_LAMERLEMMA_SPEC;
};

#endif
