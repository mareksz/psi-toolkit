#include "gobio.hpp"


#include <sstream>

#include "exceptions.hpp"

#include "put_zsyntree_into_lattice.hpp"

Annotator* Gobio::Factory::doCreateAnnotator(
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

    std::string terminalTag = options["terminal-tag"].as<std::string>();
    int edgeNumberLimit = options["edge-number-limit"].as<int>();

    return new Gobio(lang, rulesPathString, terminalTag, edgeNumberLimit);
}

void Gobio::Factory::doAddLanguageIndependentOptionsHandled(
    boost::program_options::options_description& optionsDescription
) {
    optionsDescription.add_options()
        ("edge-number-limit",
        boost::program_options::value<int>()->default_value(DEFAULT_EDGE_NUMBER_LIMIT),
        "maximal number of edges inserted between each two vertices")
        ("rules",
        boost::program_options::value<std::string>()->default_value(DEFAULT_RULE_FILE),
        "file with rules in text format")
        ("terminal-tag",
         boost::program_options::value<std::string>()->default_value(DEFAULT_TERMINAL_TAG),
         "tag for terminal");
}

std::string Gobio::Factory::doGetName() const {
    return "gobio";
}

std::string Gobio::Factory::doGetSubType() const {
    return "parser";
}

boost::filesystem::path Gobio::Factory::doGetFile() const {
    return __FILE__;
}

std::list<std::list<std::string> > Gobio::Factory::doRequiredLayerTags() {
    return boost::assign::list_of
        (boost::assign::list_of(std::string("gobio-tagset")))
        (boost::assign::list_of(DEFAULT_TERMINAL_TAG));
}

std::list<std::list<std::string> > Gobio::Factory::doRequiredLayerTags(
    const boost::program_options::variables_map& options) {

    if (options.count("terminal-tag")) {
        std::string terminalTag = options["terminal-tag"].as<std::string>();

        return boost::assign::list_of
            (boost::assign::list_of(std::string("gobio-tagset")))
            (boost::assign::list_of(std::string(terminalTag)));
    }

    return doRequiredLayerTags();
}


std::list<std::list<std::string> > Gobio::Factory::doOptionalLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::string> Gobio::Factory::doProvidedLayerTags() {
    std::list<std::string> layerTags;
    layerTags.push_back("gobio");
    layerTags.push_back("parse");
    return layerTags;
}

const std::string Gobio::Factory::DEFAULT_RULE_FILE
    = "%ITSDATA%/%LANG%/rules.g";

const std::string Gobio::Factory::DEFAULT_TERMINAL_TAG
    = "parse-terminal";

const int Gobio::Factory::DEFAULT_EDGE_NUMBER_LIMIT
    = -1;

LatticeWorker* Gobio::doCreateLatticeWorker(Lattice & lattice) {
    return new Worker(*this, lattice);
}

Gobio::Worker::Worker(Gobio & processor, Lattice & lattice) :
    LatticeWorker(lattice), processor_(processor)
{ }

void Gobio::Worker::doRun() {
    processor_.parse(lattice_);
}

std::string Gobio::doInfo() {
    return "gobio parser";
}

Gobio::Gobio(
    std::string lang,
    std::string rulesPath,
    std::string terminalTag,
    int edgeNumberLimit
) :
    rulesPath_(rulesPath),
    terminalTag_(terminalTag)
{
    if (edgeNumberLimit > -1) {
        limitChecker_.reset(new LimitChecker(edgeNumberLimit));
    } else {
        limitChecker_.reset(new GobioLimitChecker(lang));
    }
}

void Gobio::parse(Lattice & lattice) {

    LayerTagMask maskGobio = lattice.getLayerTagManager().getAlternativeMask(
        boost::assign::list_of
            (lattice.getLayerTagManager().createSingletonTagCollection("gobio"))
            (lattice.getLayerTagManager().createSingletonTagCollection(terminalTag_))
    );
    LayerTagCollection tagTerminal = lattice.getLayerTagManager().createTagCollectionFromList(
        boost::assign::list_of("gobio")("parse-terminal")
    );
    LayerTagCollection tagParse = lattice.getLayerTagManager().createTagCollectionFromList(
        boost::assign::list_of("gobio")("parse")
    );

    // Add edges with '@...' categories.

    Lattice::EdgesSortedBySourceIterator ei = lattice.edgesSortedBySource(maskGobio);
    while (ei.hasNext()) {
        Lattice::EdgeDescriptor edge = ei.next();
        std::stringstream categorySs;
        categorySs << "'@" << lattice.getAnnotationText(edge) << "'";
        AnnotationItem annotationItem(
            lattice.getEdgeAnnotationItem(edge),
            categorySs.str());
        Lattice::EdgeSequence::Builder builder(lattice);
        builder.addEdge(edge);
        try {
            Lattice::EdgeDescriptor addedEdge = lattice.addEdge(
                lattice.getEdgeSource(edge),
                lattice.getEdgeTarget(edge),
                annotationItem,
                tagTerminal,
                builder.build());
        } catch (EdgeSelfReferenceException) {
            Lattice::EdgeDescriptor addedEdge = lattice.addEdge(
                lattice.getEdgeSource(edge),
                lattice.getEdgeTarget(edge),
                annotationItem,
                tagTerminal,
                Lattice::EdgeSequence());
        }
    }

    // Do parsing.

    zsymbolfactory * sym_fac = lattice.getAnnotationItemManager().getSymbolFactory();

    AnnotationItemManager & aim = lattice.getAnnotationItemManager();

    Combinator combinator(aim);

    combinator.add_rules(rulesPath_);

    AV_AI_Converter av_ai_converter(
        aim,
        combinator.get_symbol_registrar(),
        combinator.get_attribute_registrar(),
        true
    );

    Chart ch(lattice, av_ai_converter, terminalTag_, *limitChecker_);
    Agenda agenda;
    Parser parser(ch, combinator, agenda);

    parser.run();

    boost::shared_ptr< longest_left_to_right_chooser<Chart, Combinator> > chr(
        new longest_left_to_right_chooser<Chart, Combinator>()
    );

    std::vector<Combinator::rule_holder> local_rules;
    std::vector<Edge> choosen_edges = chr->go(ch, combinator, local_rules);

    std::vector<zvalue> results;
    BOOST_FOREACH(Edge edge, choosen_edges) {
        zvalue zv = edgeToZsyntree_(
            ch,
            combinator,
            edge,
            local_rules,
            sym_fac,
            lattice.getAnnotationItemManager().getZObjectsHolderPtr()
        );
        results.push_back(zv);
    }

    BOOST_FOREACH(zvalue zv, results) {
        putZsyntreeIntoLattice(
            lattice,
            tagParse,
            ZSYNTREEC(zv)
        );
    }

}


zvalue Gobio::edgeToZsyntree_(
    Chart & ch,
    Combinator & combinator,
    Edge edge,
    std::vector<Combinator::rule_holder> & local_rules,
    zsymbolfactory * sym_fac,
    zobjects_holder * holder
) {
    std::pair<Chart::partition_iterator, Chart::partition_iterator> pits
        = ch.edge_partitions(edge);

    if (pits.first == pits.second) {
        return NULL_ZVALUE;
    }

    Chart::partition_iterator pit = pits.first;

    boost::shared_ptr< tree_specification<Atom> > core_spec = combinator.tree_spec(
        ch.partition_rule_id(pit),
        ch.partition_tree_choice(pit),
        local_rules);

    assert(core_spec);

    return edgeToZsyntreeWithSpec_(
        ch,
        combinator,
        edge,
        pit,
        local_rules,
        core_spec,
        true,
        sym_fac,
        holder);
}


zvalue Gobio::edgeToZsyntreeWithSpec_(
    Chart & ch,
    Combinator & combinator,
    Edge edge,
    Chart::partition_iterator pit,
    std::vector<Combinator::rule_holder> & local_rules,
    boost::shared_ptr< tree_specification<zvalue> > spec,
    bool is_main,
    zsymbolfactory * sym_fac,
    zobjects_holder * holder
) {
    zsyntree * result = zsyntree::generate(holder);

    boost::shared_ptr< tree_branch<Atom, Chart, Combinator::equivalent_type> > tb
        = extract_tree_branch_with_spec<Atom, Chart, Combinator>(
            ch,
            edge,
            pit,
            combinator,
            local_rules,
            spec,
            is_main);

    zsymbol * zcat = sym_fac->get_symbol(
        combinator.get_master().string_representation(tb->root()).c_str());
    if (strcmp(zcat->to_string(), "NULL_ZVALUE")) {
        result->setCategory(zcat);
    } else if(tb->is_supported()) {
        result->setCategory(
            sym_fac->get_symbol(
                leafSymbolToCategory_(
                    combinator.get_symbol_registrar().get_obj(
                        ch.edge_category(
                            tb->supporting_edge()).get_cat())).c_str()));
    } else
        result->setCategory(sym_fac->get_symbol("???"));

    if (tb->is_supported()) {
        result->setSegmentInfo(
            ch.edge_source(tb->supporting_edge()),
            ch.edge_target(tb->supporting_edge()) - ch.edge_source(tb->supporting_edge()));

        result->setOrigin(tb->supporting_edge());

        Atom def = combinator.get_master().false_value();
        const Category & avm = ch.edge_category(tb->supporting_edge());
        for (int ai = 0; ai < avm.nb_attrs(); ++ai) {
            if (combinator.get_master().is_true(avm.get_attr(ai, def))) {
                result->setAttr(
                    sym_fac->get_symbol(
                        combinator.get_attribute_registrar().get_obj(ai).c_str()),
                        avm.get_attr(ai, def));
            }
        }
    }

    for (size_t i = 0; i < tb->nb_children(); ++i) {
        zvalue sub_zv = edgeToZsyntreeWithSpec_(
            ch,
            combinator,
            tb->child_edge(i),
            ch.edge_partitions(tb->child_edge(i)).first,
            local_rules,
            tb->child_spec(i),
            false,
            sym_fac,
            holder);
        if (ZSYNTREEP(sub_zv)) {
            result->addSubtree(
                ZSYNTREEC(sub_zv),
                (combinator.get_master().is_true(tb->child_label(i))
                ? sym_fac->get_symbol(
                    combinator.get_master().string_representation(tb->child_label(i)).c_str())
                : NULL));
        }
    }

    return result;
}

std::string Gobio::leafSymbolToCategory_(
    const std::string& symbol) {

    if (symbol == "flicz")                         return "LG";
    if (symbol == "rzeczownik")                    return "R";
    if (symbol == "czasownik")                     return "C";
    if (symbol == "przymiotnik")                   return "P";
    if (symbol == "przysłówek")                    return "PS";
    if (symbol == "przyimek")                      return "PR";
    if (symbol == "przyimek_spośród")              return "PR";
    if (symbol == "spójnik")                       return "S";
    if (symbol == "zaimek_dzierżawczy")            return "ZP";
    if (symbol == "zaimek_przysłowny")             return "ZS";
    if (symbol == "zaimek_rzeczowny_nieokreślony") return "ZRn";
    if (symbol == "zaimek_rzeczowny_osobowy")      return "ZRo";
    if (symbol == "zaimek_wskazujący")             return "ZRw";

    if (symbol == "czasownik_być") return "'$być'";

    if (symbol == "przecinek") return "','";
    if (symbol == "uuto")      return "'to'";

    if (symbol.length() >= 2 && symbol[0] == 'z' && symbol[1] == 'z') {
        std::stringstream resSs;
        resSs << "'" << symbol.substr(2) << "'";
        return resSs.str();
    }

    return symbol;
}

double Gobio::doGetQualityScore(
    const boost::program_options::variables_map & /*options*/) const {

    return -9999;
}
