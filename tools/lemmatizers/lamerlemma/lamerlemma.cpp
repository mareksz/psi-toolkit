#include "lamerlemma.hpp"

#include "logging.hpp"

LamerLemma::LamerLemma(const boost::program_options::variables_map& options)
 : m_dict(options.count("morpho"))
{
    if(options.count("lang")) {
        std::cerr << options["lang"].as<std::string>() << std::endl;
    }
    
    if(options.count("loadbin")) {
        std::string dictionary_file = options["loadbin"].as<std::string>();
        m_dict.load(dictionary_file);
    }
    else if(options.count("loadtxt")) {
        std::string dictionary_text = options["loadtxt"].as<std::string>();
        m_dict.read_dictionary(dictionary_text);
    }
    
    if(options.count("savebin")) {
        std::string savebin = options["savebin"].as<std::string>();
        m_dict.save(savebin);
    }
}

bool LamerLemma::lemmatize(const std::string& token,
                               AnnotationItemManager& annotationItemManager,
                               LemmatizerOutputIterator& outputIterator) {
    bool foundLemma = false;
    
    DictionaryItem item = m_dict.look_up(token);
    if(item.get_pos() != "unknown")
        foundLemma = true;
    else
        return false;
        
    Interpretations interpretations = item.get_interpretations();
    for(Interpretations::iterator it = interpretations.begin();
        it != interpretations.end(); it++) {
        
        std::string lemma = it->get_lemma();
        outputIterator.addLemma(lemma);
        
        if(m_dict.has_pos()) {
            std::string pos = it->get_pos();
            AnnotationItem lexeme(pos, StringFrag(lemma));
    
            if(m_dict.has_morpho()) {
                std::string morpho = it->get_morpho();
                annotationItemManager.setValue(lexeme, "morpho", morpho);
            }    
            outputIterator.addLexeme(lexeme);
        }
    }
    
    return foundLemma;
}

boost::program_options::options_description LamerLemma::optionsHandled() {
    boost::program_options::options_description desc;

    desc.add_options()
        ("loadtxt", boost::program_options::value<std::string>(), "Read text file")
        ("morpho", "Text file includes morphology")
        ("savebin", boost::program_options::value<std::string>(), "Save binary dictionary file")
        ("loadbin", boost::program_options::value<std::string>(), "Load binary dictionary file");
    return desc;
}


std::string LamerLemma::getName() {
    return "lamerlemma";
}

boost::filesystem::path LamerLemma::getFile() {
    return __FILE__;
}

std::string LamerLemma::getLanguage() const {
    return "pl";
}

AnnotatorFactory::LanguagesHandling LamerLemma::languagesHandling(
    const boost::program_options::variables_map& /*options*/) {
    return AnnotatorFactory::JUST_ONE_LANGUAGE;
}

std::list<std::string> LamerLemma::languagesHandled(
    const boost::program_options::variables_map& /*options*/) {
    return boost::assign::list_of(std::string("pl"));
}

bool LamerLemma::checkRequirements(
    const boost::program_options::variables_map& /*options*/,
    std::ostream & /*message*/) {

    return true;
}

std::list<std::string> LamerLemma::getLayerTags() {
    std::list<std::string> layerTags;

    layerTags.push_back("lamerlemma");

    return layerTags;
}
