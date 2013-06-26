#include "bonsai_runner.hpp"
#include "config.hpp"

#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/bind.hpp>

#include "bi_lang_specific_processor_file_fetcher.hpp"
#include "bi_language_dependent_annotator_factory.hpp"

#include "bonsai.hpp"

BonsaiRunner::BonsaiRunner(const boost::program_options::variables_map& options)
{
    std::string lang = options["lang"].as<std::string>();
    langCode_ = lang;

    std::string trg_lang = options["trg-lang"].as<std::string>();

    createTags_(trg_lang);

    BiLangSpecificProcessorFileFetcher fileFetcher(__FILE__, lang, trg_lang);

    configFile_ =
        fileFetcher.getOneFile(
            options["config"].as<std::string>());
        
    boost::program_options::variables_map cfg;
    if (options.count("config")) {
	std::ifstream configStream(configFile_.string().c_str());
	boost::program_options::store(boost::program_options::parse_config_file(configStream, optionsHandled()), cfg);
        boost::program_options::notify(cfg);
    }

    std::vector<std::string> rs_files;
    std::vector<std::string> lm_files;

    if (options.count("rs")) {
        rs_files = normalizePaths_(configFile_,
                                   options["rs"].as<std::vector<std::string> >());
    }
    else {
	if (cfg.count("rs")) {
		rs_files = normalizePaths_(configFile_,
                                           cfg["rs"].as<std::vector<std::string> >());
	}
	else {
	    std::cerr << "Error. No translation rules sets given. Terminating." << std::endl;
	    //exit(1);
	    // Co zamiast exitow? Wyjatki
	}
    }
    
    if (options.count("lm")) {
        lm_files = normalizePaths_(configFile_,
                                   options["lm"].as<std::vector<std::string> >());
    }
    else {
	if (cfg.count("lm")) {
	    lm_files = normalizePaths_(configFile_,
                                       cfg["lm"].as<std::vector<std::string> >());
	}
	else {
	    std::cerr << "Error. No language models given. Terminating" << std::endl;
	    //exit(1);
	}
    }

    poleng::bonsai::Floats tm_weights;
    double word_penalty_weight = 0;
    poleng::bonsai::Floats rs_weights;
    poleng::bonsai::Floats lm_weights;

    if (options.count("tm_weight")) {
	tm_weights = options["tm_weight"].as<poleng::bonsai::Floats>();
    }
    else if (cfg.count("tm_weight")) {
	tm_weights = cfg["tm_weight"].as<poleng::bonsai::Floats>();
    }
    else {
	std::cerr << "Warning. No translation model weights given. Parameters will be weighted equally." << std::endl;
    }

    if (options.count("rs_weight")) {
	rs_weights = options["rs_weight"].as<poleng::bonsai::Floats>();
    }
    else if (cfg.count("rs_weight")) {
	rs_weights = cfg["rs_weight"].as<poleng::bonsai::Floats>();
    }
    else {
	std::cerr << "Warning. No rule set weights given. Rule sets will be weighted equally." << std::endl;
    }

    if (options.count("lm_weight")) {
	lm_weights = options["lm_weight"].as<poleng::bonsai::Floats>();
    }
    else if (cfg.count("lm_weight")) {
	lm_weights = cfg["lm_weight"].as<poleng::bonsai::Floats>();
    }
    else {
	std::cerr << "Warning. No language model weights given. Language models will be weighted equally." << std::endl;
    }

    if (options.count("word_penalty")) {
	word_penalty_weight = options["word_penalty"].as<double>();
    }
    else if (cfg.count("word_penalty")) {
	word_penalty_weight = cfg["word_penalty"].as<double>();
    }
    else {
	std::cerr << "Warning. No word penalty given. Using 0 as word penalty weight." << std::endl;
	word_penalty_weight = 0;
    }

    if (rs_weights.size() != rs_files.size()) {
	std::cerr << "Warning. Number of rule sets and rule set weights differs. Rule sets will be weighted equally." << std::endl;
	rs_weights.clear();
	rs_weights.resize(rs_files.size(), 1/rs_files.size());
    }

    if (lm_weights.size() != lm_files.size()) {
	std::cerr << "Warning. Number of language models and language model weights differs. Language models will be weighted equally." << std::endl;
	lm_weights.clear();
	lm_weights.resize(lm_files.size(), 1/lm_files.size());
    }

    poleng::bonsai::Translator::set_tm_weights(tm_weights);
    poleng::bonsai::Translator::set_rs_weights(rs_weights);
    poleng::bonsai::Translator::set_lm_weights(lm_weights);
    poleng::bonsai::Translator::set_word_penalty_weight(word_penalty_weight);

    poleng::bonsai::LmContainerPtr lmc ( new poleng::bonsai::LmContainer() );
    for (size_t i = 0; i < lm_files.size(); i++) {
        poleng::bonsai::LanguageModelPtr lm( new poleng::bonsai::LanguageModel(lm_files[i]) );
	if (i == 0)
	    lm->set_lc(true);

        lmc->add_lm(lm);
    }

    poleng::bonsai::RuleLoaderPtr rl( new poleng::bonsai::RuleLoader(7, 4, lmc) );

    rl->set_verbosity(options["verbose"].as<int>());
    rl->set_max_transformations_per_hyperedge(options["max_trans"].as<int>());
    rl->set_max_hyperedges_per_nonterminal(options["max_hyper"].as<int>());
    rl->set_max_transformation_factor(options["eps"].as<double>());

    for (size_t i = 0; i < rs_files.size(); i++) {
	poleng::bonsai::RuleSetPtr rs( new poleng::bonsai::RuleSet( rs_files[i], 7, 4, i, lmc ) );
	rl->add_rule_set( rs );
    }

    translator_.reset(
	new poleng::bonsai::Translator(
	    rl,
	    lmc,
	    options["nbest"].as<int>(),
	    options["stacksize"].as<int>()
	)
    );

    translator_->set_verbosity(options["verbose"].as<int>());
    translator_->set_pedantry(options.count("pedantic") > 0);
    translator_->set_mert(options.count("mert") > 0);
    translator_->set_nbest(options["nbest"].as<int>());
}

std::string BonsaiRunner::getName() {
    return "bonsai";
}

std::string BonsaiRunner::getSubType() {
    return "translator";
}

boost::filesystem::path BonsaiRunner::getFile() {
    return __FILE__;
}

AnnotatorFactory::LanguagesHandling BonsaiRunner::languagesHandling(
    const boost::program_options::variables_map& options) {

    return LanguageDependentAnnotatorFactory::checkLangOption(options);
}

std::list<std::string> BonsaiRunner::languagesHandled(
    const boost::program_options::variables_map& options) {

    if (LanguageDependentAnnotatorFactory::checkLangOption(options)
        == AnnotatorFactory::JUST_ONE_LANGUAGE)
        return boost::assign::list_of(options["lang"].as<std::string>());

    std::string trgLang =
        options.count("trg-lang")
        ? options["trg-lang"].as<std::string>()
        : "";

    std::string fileSuffix = trgLang + ".cfg";
    
    std::vector<std::string> langs;
    
    boost::filesystem::path dataDirectory = getItsData(getFile());
    
    boost::filesystem::directory_iterator end_iter;
    for (boost::filesystem::directory_iterator fiter(dataDirectory);
         fiter != end_iter;
         ++fiter) {
        boost::filesystem::path seg(fiter->path().filename());
        std::string lexiconFileName = seg.string();
    
        if (lexiconFileName.length() > fileSuffix.length()
            && lexiconFileName.substr(
                lexiconFileName.length() - fileSuffix.length())
            == fileSuffix) {
            if (trgLang.empty()) {
                if (lexiconFileName.length() == 4 + fileSuffix.length())
                    langs.push_back(lexiconFileName.substr(0, 2));
            } else {
                langs.push_back(lexiconFileName.substr(
                                    0, lexiconFileName.length() - fileSuffix.length()));
    
            }
        }
    }

    std::sort(langs.begin(), langs.end());

    return std::list<std::string>(langs.begin(), langs.end());
}

boost::program_options::options_description BonsaiRunner::optionsHandled() {
    boost::program_options::options_description desc(ProcessorFactory::OPTION_LABEL);

    BiLanguageDependentAnnotatorFactory::addBiLanguageDependentOptions(desc);

    desc.add_options()
        ("config", boost::program_options::value<std::string>()
         ->default_value(DEFAULT_CONFIG_FILE), "Path to configuration")
	("rs", boost::program_options::value<std::vector<std::string> >(),
         "Paths to translation rule sets")
	("lm", boost::program_options::value<std::vector<std::string> >(),
         "Paths to language models")
    
	("stacksize", boost::program_options::value<int>()->default_value(20),
         "Node translation stack size")
	("max_trans", boost::program_options::value<int>()->default_value(20),
         "Maximal number of transformations per hyper edge")
	("max_hyper", boost::program_options::value<int>()->default_value(20),
         "Maximal number of hyper edges per symbol")
	("eps", boost::program_options::value<double>()->default_value(-1),
         "Allowed transformation cost factor")
	("nbest", boost::program_options::value<int>()->default_value(1),
         "Display n best translations")
    
	("verbose", boost::program_options::value<int>()->default_value(0),
         "Level of verbosity: 0, 1, 2")
	("pedantic", "Pedantic cost calculation (for debugging)")
	("mert", "Output for MERT (combine with nbest)")
    
	("tm_weight", boost::program_options::value<poleng::bonsai::Floats>(),
         "Weights for translation model parameters")
        ("rs_weight", boost::program_options::value<poleng::bonsai::Floats>(),
         "Weights for different translation rules sets")
        ("lm_weight", boost::program_options::value<poleng::bonsai::Floats>(),
         "Weights for different language models")
        ("word_penalty", boost::program_options::value<double>(),
         "Weight for word penalty")
    ;
    return desc;
}

std::list<std::string> BonsaiRunner::providedLayerTags() {
    return mainLayerTags_();
}

std::list<std::list<std::string> > BonsaiRunner::requiredLayerTags() {
    return
        boost::assign::list_of(
            boost::assign::list_of(std::string("parse")));
}

std::string BonsaiRunner::getContinuation(
    const boost::program_options::variables_map& options) {

    if (options.count("trg-lang")) {
        std::string trgLang = options["trg-lang"].as<std::string>();

        return "simple-writer --tags trans";
    }

    return "psi-toolkit";
}

std::list<std::string> BonsaiRunner::tagsToOperateOn() {
    return boost::assign::list_of
        (std::string("!parse"))(std::string("parse"))(LayerTagManager::getLanguageTag(langCode_));
}

void BonsaiRunner::processEdge(Lattice& lattice, Lattice::EdgeDescriptor edge) {
    boost::shared_ptr<Bonsai> Bonsai_;
    
    Bonsai_.reset(new Bonsai(translator_, langCode_));
    Bonsai_->translate(lattice, edge);
}

std::list<std::string> BonsaiRunner::mainLayerTags_() {
    std::list<std::string> tags = coreLayerTags_();

    tags.push_back("parse-transferred");

    return tags;
}

std::list<std::string> BonsaiRunner::coreLayerTags_() {
    return boost::assign::list_of
        (std::string("bonsai"))
        (std::string("!translation"));
}

void BonsaiRunner::createTags_(const std::string& trgLang) {
    std::string trgLangCode = LayerTagManager::getLanguageTag(trgLang);

    tags_ = mainLayerTags_();
    tags_.push_back(trgLangCode);
    
    targetFormTags_ = coreLayerTags_();
    targetFormTags_.push_back("form");
    targetFormTags_.push_back(trgLangCode);
    
    targetTokenTags_ = coreLayerTags_();
    targetTokenTags_.push_back("token");
    targetTokenTags_.push_back(trgLangCode);
}

std::vector<std::string> BonsaiRunner::normalizePaths_(
    boost::filesystem::path &config,
    const std::vector<std::string> &paths) {
    
    std::vector<std::string> outPaths;
    BOOST_FOREACH(std::string path, paths) {
        boost::filesystem::path temp(path);
	// @TODO: use something nicer when new boost becomes minimal requirement
        // if (temp.is_absolute())
	if(temp.string().at(0) == '/')
            outPaths.push_back(path);
        else {
            boost::filesystem::path parent = config.parent_path();
            outPaths.push_back(parent.string() + "/" + temp.string()); // EVIL, but path::append not working?
        }
    }
    return outPaths;
}

const std::string BonsaiRunner::DEFAULT_CONFIG_FILE
= "%ITSDATA%/%LANG%%TRGLANG%/%LANG%%TRGLANG%.cfg";
