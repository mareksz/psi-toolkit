#include "unumsunt.hpp"


#include <algorithm>
#include <cctype>
#include <fstream>
#include <list>
#include <sstream>
#include <string>
#include <utility>

#include <boost/algorithm/string/trim.hpp>
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
    UnumsuntRuleGrammar rGrammar;
    UnumsuntAssignmentGrammar aGrammar;
    std::ifstream rulesFs(rulesPath.c_str());
    std::string line;
    int lineno = 0;
    while (rulesFs.good()) {
        ++lineno;
        std::getline(rulesFs, line);
        boost::algorithm::trim(line);
        if (line.empty()) break;
        switch (line.at(0)) {
            case '#': {
                // comment
                break;
            }
            case '@': {
                // hash elements
                std::stringstream lineSs(line.substr(1));
                std::string type;
                lineSs >> type;
                if (type == "source") {
                    lineSs >> sourceTagset_;
                } else if (type == "target") {
                    lineSs >> targetTagset_;
                } else if (type == "tags") {
                    while (lineSs.good()) {
                        std::string tag;
                        lineSs >> tag;
                        preservedTags_.push_back(tag);
                    }
                } else {
                    std::string source;
                    std::string target;
                    lineSs >> source >> target;
                    if (type == "cat") {
                        cat_map_.insert(StringPair(source, target));
                    } else if (type == "attr") {
                        attr_map_.insert(StringPair(source, target));
                    } else if (type == "val") {
                        val_map_.insert(StringPair(source, target));
                    } else {
                        std::stringstream errorSs;
                        errorSs << "Tagset converter: unknown command (@" << type <<
                            ") in line " << lineno;
                        throw FileFormatException(errorSs.str());
                    }
                }
                break;
            }
            default: {
                // auxiliary rule
                line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
                UnumsuntRuleItem rItem;
                std::string::const_iterator rBegin = line.begin();
                std::string::const_iterator rEnd = line.end();
                if (parse(rBegin, rEnd, rGrammar, rItem)) {
                    UnumsuntAssignmentItem aItem;
                    std::string::const_iterator aBegin = rItem.condition.begin();
                    std::string::const_iterator aEnd = rItem.condition.end();
                    if (parse(aBegin, aEnd, aGrammar, aItem)) {
                        DEBUG("Condition: [" << aItem.arg << "] == [" << aItem.val << "]");
                        std::pair<UnumsuntRulesMap::iterator, bool> mapInsertionResult(
                            aux_rules_map_.insert(UnumsuntRulesMapItem(
                                StringPair(aItem.arg, aItem.val),
                                std::vector<StringPair>()
                            ))
                        );
                        BOOST_FOREACH(std::string command, rItem.commands) {
                            UnumsuntAssignmentItem aItem;
                            std::string::const_iterator aBegin = command.begin();
                            std::string::const_iterator aEnd = command.end();
                            if (parse(aBegin, aEnd, aGrammar, aItem)) {
                                DEBUG("Command: [" << aItem.arg << "] := [" << aItem.val << "]");
                                mapInsertionResult.first->second.push_back(
                                    StringPair(aItem.arg, aItem.val)
                                );
                            }
                        }
                    }
                } else {
                    std::stringstream errorSs;
                    errorSs << "Tagset converter: syntax error in line " << lineno << ": " << line;
                    throw FileFormatException(errorSs.str());
                }
                break;
            }
        }
    }

    DEBUG("Rules map size = " << aux_rules_map_.size());
    DEBUG("Rules in map:");
    BOOST_FOREACH(UnumsuntRulesMapItem rule, aux_rules_map_) {
        std::stringstream sstr;
        sstr << "if [" << rule.first.first << "] == [" << rule.first.second << "]";
        std::string comma(" then");
        BOOST_FOREACH(StringPair command, rule.second) {
            sstr << comma << " [" << command.first << "] := [" << command.second << "]";
            comma = ",";
        }
        DEBUG(sstr.str());
    }
}


void Unumsunt::convertTags(Lattice & lattice) {
    LayerTagMask maskSourceTagset = lattice.getLayerTagManager().getMask(sourceTagset_);
    LayerTagCollection tagTargetTagset
        = lattice.getLayerTagManager().createTagCollectionFromList(
            boost::assign::list_of("tagset-converter")(targetTagset_.c_str()));
    LayerTagCollection preservedTags
        = lattice.getLayerTagManager().createTagCollectionFromList(preservedTags_);
    Lattice::EdgesSortedBySourceIterator ei(lattice, maskSourceTagset);
    while (ei.hasNext()) {
        Lattice::EdgeDescriptor edge = ei.next();
        LayerTagCollection edgePreservedTags
            = createIntersection(lattice.getEdgeLayerTags(edge), preservedTags);
        LayerTagCollection targetTags
            = createUnion(edgePreservedTags, tagTargetTagset);
        AnnotationItem sourceAI = lattice.getEdgeAnnotationItem(edge);
        std::string sourceCategory = sourceAI.getCategory();
        std::string targetCategory;
        std::map<std::string, std::string>::iterator cmi = cat_map_.find(sourceCategory);
        if (cmi == cat_map_.end()) {
            targetCategory = sourceCategory;
        } else {
            targetCategory = cmi->second;
        }
        AnnotationItem targetAI(targetCategory, sourceAI.getTextAsStringFrag());
        std::list< std::pair<std::string, zvalue> > avs
            = lattice.getAnnotationItemManager().getValuesAsZvalues(sourceAI);
        typedef std::pair<std::string, zvalue> AVPair;
        BOOST_FOREACH(AVPair av, avs) {
            std::map<std::string, std::string>::iterator ami = attr_map_.find(av.first);
            std::map<std::string, std::string>::iterator vmi = val_map_.find(
                lattice.getAnnotationItemManager().zvalueToString(av.second)
            );
            if (ami == attr_map_.end() && vmi == val_map_.end()) {
                lattice.getAnnotationItemManager().setValue(targetAI, av.first, av.second);
            } else if (ami != attr_map_.end() && vmi == val_map_.end()) {
                lattice.getAnnotationItemManager().setValue(targetAI, ami->second, av.second);
            } else if (ami == attr_map_.end() && vmi != val_map_.end()) {
                lattice.getAnnotationItemManager().setValue(targetAI, av.first, vmi->second);
            } else {
                lattice.getAnnotationItemManager().setValue(targetAI, ami->second, vmi->second);
            }
        }
        const std::list<Lattice::Partition> partitions = lattice.getEdgePartitions(edge);
        BOOST_FOREACH(Lattice::Partition partition, partitions) {
            Lattice::EdgeSequence::Builder builder(lattice);
            Lattice::Partition::Iterator pi(lattice, partition);
            while (pi.hasNext()) {
                Lattice::EdgeDescriptor partitionEdge = pi.next();
                std::map<Lattice::EdgeDescriptor, Lattice::EdgeDescriptor>::iterator ri
                    = replacements_.find(partitionEdge);
                if (ri == replacements_.end()) {
                    builder.addEdge(partitionEdge);
                } else {
                    builder.addEdge(ri->second);
                }
            }
            replacements_.insert(std::pair<Lattice::EdgeDescriptor, Lattice::EdgeDescriptor>(
                edge,
                lattice.addEdge(
                    lattice.getEdgeSource(edge),
                    lattice.getEdgeTarget(edge),
                    targetAI,
                    targetTags,
                    builder.build()
                )
            ));
        }
    }
}
