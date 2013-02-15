#include "unumsunt.hpp"


#include <fstream>
#include <sstream>
#include <string>
#include <utility>

#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>

#include "annotation_item.hpp"
#include "annotation_item_manager.hpp"
#include "zvalue.hpp"


Annotator* Unumsunt::Factory::doCreateAnnotator(
    const boost::program_options::variables_map & options
) {
    std::string lang = options["lang"].as<std::string>();
    LangSpecificProcessorFileFetcher fileFetcher(__FILE__, lang);

    std::string rulesPathString;

    if (options.count("rules")) {
        std::string rulesFilename = options["rules"].as<std::string>();
        boost::filesystem::path rulesPath = fileFetcher.getOneFile(rulesFilename);
        rulesPathString = rulesPath.string();
    }

    return new Unumsunt(lang, rulesPathString);
}


void Unumsunt::Factory::doAddLanguageIndependentOptionsHandled(
    boost::program_options::options_description& optionsDescription
) {
    optionsDescription.add_options()
        ("rules",
        boost::program_options::value<std::string>()->default_value(DEFAULT_RULE_FILE),
        "rules file");
}


std::string Unumsunt::Factory::doGetName() const {
    return "tagset-converter";
}


boost::filesystem::path Unumsunt::Factory::doGetFile() const {
    return __FILE__;
}


std::list< std::list<std::string> > Unumsunt::Factory::doRequiredLayerTags() {
    return std::list< std::list<std::string> >();
}


std::list< std::list<std::string> > Unumsunt::Factory::doOptionalLayerTags() {
    return std::list< std::list<std::string> >();
}


std::list<std::string> Unumsunt::Factory::doProvidedLayerTags() {
    std::list<std::string> layerTags;
    layerTags.push_back("tagset-converter");
    return layerTags;
}


const std::string Unumsunt::Factory::DEFAULT_RULE_FILE
    = "%ITSDATA%/%LANG%.u";


LatticeWorker* Unumsunt::doCreateLatticeWorker(Lattice & lattice) {
    return new Worker(*this, lattice);
}


Unumsunt::Worker::Worker(Unumsunt & processor, Lattice & lattice) :
    LatticeWorker(lattice), processor_(processor)
{ }


void Unumsunt::Worker::doRun() {
    processor_.convertTags(lattice_);
}


std::string Unumsunt::doInfo() {
    return "unumsunt tagset converter";
}


Unumsunt::Unumsunt(
    std::string langCode,
    std::string rulesPath
) :
    langCode_(langCode)
{
    UnumsuntConversionGrammar grammar;
    std::ifstream rulesFs(rulesPath.c_str());
    std::string line;
    while (rulesFs.good()) {

        std::string source;
        std::string target;

        std::getline(rulesFs, line);

        UnumsuntConversionItem item;
        std::string::const_iterator begin = line.begin();
        std::string::const_iterator end = line.end();

        if (parse(begin, end, grammar, item)) {
            source = item.source;
            target = item.target;
        } else {
            std::stringstream pairSs(line);
            pairSs >> source >> target;
        }

        symbol_map_.insert(std::pair<std::string, std::string>(source, target));

    }
}


void Unumsunt::convertTags(Lattice & lattice) {
    LayerTagMask maskSourceTagset = lattice.getLayerTagManager().getMaskWithLangCode(
        getSourceTagset_(),
        langCode_
    );
    Lattice::EdgesSortedBySourceIterator ei(lattice, maskSourceTagset);
    while (ei.hasNext()) {
        Lattice::EdgeDescriptor edge = ei.next();
        LayerTagCollection tagTargetTagset
            = lattice.getLayerTagManager().createTagCollectionFromListWithLangCode(
                boost::assign::list_of("tagset-converter")(getTargetTagset_().c_str()),
                langCode_
            );
        AnnotationItem sourceAI = lattice.getEdgeAnnotationItem(edge);
        std::string sourceCategory = sourceAI.getCategory();
        std::string targetCategory;
        std::map<std::string, std::string>::iterator smi = symbol_map_.find(sourceCategory);
        if (smi == symbol_map_.end()) {
            targetCategory = sourceCategory;
        } else {
            targetCategory = smi->second;
        }
        AnnotationItem targetAI(targetCategory, sourceAI.getTextAsStringFrag());
        std::list< std::pair<std::string, zvalue> > avs
            = lattice.getAnnotationItemManager().getValuesAsZvalues(sourceAI);
        typedef std::pair<std::string, zvalue> AVPair;
        BOOST_FOREACH(AVPair av, avs) {
            std::map<std::string, std::string>::iterator smi1 = symbol_map_.find(av.first);
            std::map<std::string, std::string>::iterator smi2 = symbol_map_.find(
                lattice.getAnnotationItemManager().zvalueToString(av.second)
            );
            if (smi1 == symbol_map_.end() && smi2 == symbol_map_.end()) {
                lattice.getAnnotationItemManager().setValue(targetAI, av.first, av.second);
            } else if (smi1 != symbol_map_.end() && smi2 == symbol_map_.end()) {
                lattice.getAnnotationItemManager().setValue(targetAI, smi1->second, av.second);
            } else if (smi1 == symbol_map_.end() && smi2 != symbol_map_.end()) {
                lattice.getAnnotationItemManager().setValue(targetAI, av.first, smi2->second);
            } else {
                lattice.getAnnotationItemManager().setValue(targetAI, smi1->second, smi2->second);
            }
        }
        lattice.addEdge(
            lattice.getEdgeSource(edge),
            lattice.getEdgeTarget(edge),
            targetAI,
            tagTargetTagset,
            lattice.getEdgePartitions(edge).front().getSequence()
        );
    }
}


std::string Unumsunt::getSourceTagset_() const {
    if (langCode_ == "pl") return "morfologik-tagset";
    return "morfologik-tagset";
}


std::string Unumsunt::getTargetTagset_() const {
    if (langCode_ == "pl") return "gobio-tagset";
    return "gobio-tagset";
}

