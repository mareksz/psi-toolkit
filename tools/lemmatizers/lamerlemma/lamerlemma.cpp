#include "lamerlemma.hpp"

#include "logging.hpp"

LamerLemma::LamerLemma(const boost::program_options::variables_map& options)
 : m_dict(options.count("pos") || options.count("morpho"), options.count("morpho"))
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
    
    typedef std::map<std::string, std::vector<Interpretation> > StringInterMap;
    StringInterMap lemma_map;
    BOOST_FOREACH(Interpretation i , interpretations)
        lemma_map[i.get_lemma()].push_back(i);
    
    BOOST_FOREACH(StringInterMap::value_type lem_inter, lemma_map)
    {
        std::string lemma = lem_inter.first;
        outputIterator.addLemma(lemma);
        
        if(m_dict.has_pos()) {
            StringInterMap lexeme_map;
            BOOST_FOREACH(Interpretation i, lem_inter.second)
            {
                std::string lemma = i.get_lemma();
                std::string pos = i.get_pos();
                std::string lexeme = lemma + "+" + pos;
                lexeme_map[lexeme].push_back(i);
            }
            
            BOOST_FOREACH(StringInterMap::value_type lex_inter, lexeme_map)
            {
                std::string lexeme = lex_inter.first;
                std::string pos = lex_inter.second[0].get_pos();
                
                AnnotationItem ai_lexeme(pos, StringFrag(lexeme));
                outputIterator.addLexeme(ai_lexeme);
        
                BOOST_FOREACH(Interpretation i, lex_inter.second) {
                    AnnotationItem form(pos, StringFrag(token));
                    if(m_dict.has_morpho()) {
                        typedef std::pair<std::string, std::string> StringPair;
                        std::vector<std::string>& morphologies = i.get_morpho();
                        BOOST_FOREACH(StringPair kv, morpho_to_features(morphologies)) {
                            annotationItemManager.setValue(form, kv.first, kv.second);
                        }
                    }
                    outputIterator.addForm(form);
                }
            }
        }
    }
    return foundLemma;
}

std::vector<std::pair<std::string, std::string> > 
LamerLemma::morpho_to_features(std::vector<std::string> &morphos) {
    size_t c = 0;
    std::vector<std::pair<std::string, std::string> > features;
    BOOST_FOREACH(std::string morpho, morphos) {
        std::stringstream key;
        key << "m" << c;
        
        int pos;
        if((pos = morpho.find("=")) != std::string::npos) {
            std::string first = morpho.substr(0, pos);
            std::string second = morpho.substr(pos+1);
            
            features.push_back(std::make_pair(first, second));    
        }
        else
            features.push_back(std::make_pair(key.str(), morpho));
        c++;
    }
    return features;
}

boost::program_options::options_description LamerLemma::optionsHandled() {
    boost::program_options::options_description desc;

    desc.add_options()
        ("loadbin", boost::program_options::value<std::string>(), "Load binary dictionary file")
        ("loadtxt", boost::program_options::value<std::string>(), "Read text file")
        ("pos", "Text file contains part-of-speech information")
        ("morpho", "Text file contains morphology information (implies --pos)")
        ("savebin", boost::program_options::value<std::string>(), "Save binary dictionary file")
    ;
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
