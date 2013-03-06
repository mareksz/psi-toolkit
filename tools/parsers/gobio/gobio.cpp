#include "gobio.hpp"


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

    return new Gobio(rulesPathString);
}

void Gobio::Factory::doAddLanguageIndependentOptionsHandled(
    boost::program_options::options_description& optionsDescription
) {
    optionsDescription.add_options()
        ("rules",
        boost::program_options::value<std::string>()->default_value(DEFAULT_RULE_FILE),
        "rules file");
}

std::string Gobio::Factory::doGetName() const {
    return "gobio";
}

boost::filesystem::path Gobio::Factory::doGetFile() const {
    return __FILE__;
}

std::list<std::list<std::string> > Gobio::Factory::doRequiredLayerTags() {
    return std::list<std::list<std::string> >();
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

Gobio::Gobio(std::string rulesPath) : rulesPath_(rulesPath) { }

void Gobio::parse(Lattice & lattice) {

    AnnotationItemManager & aim = lattice.getAnnotationItemManager();

    Combinator combinator(aim);

    combinator.add_rules(rulesPath_);

    AV_AI_Converter av_ai_converter(
        aim,
        combinator.get_symbol_registrar(),
        combinator.get_attribute_registrar(),
        true
    );

    Chart ch(lattice, av_ai_converter);
    Agenda agenda;
    Parser parser(ch, combinator, agenda);

    parser.run();

    boost::shared_ptr< longest_left_to_right_chooser<Chart, Combinator> > chr(
        new longest_left_to_right_chooser<Chart, Combinator>()
    );

    std::vector<Combinator::rule_holder> local_rules;
    std::vector<Edge> choosen_edges = chr->go(ch, combinator, local_rules);

    LayerTagMask maskGobio = lattice.getLayerTagManager().getAlternativeMask(
        boost::assign::list_of
            (lattice.getLayerTagManager().createSingletonTagCollection("gobio"))
            (lattice.getLayerTagManager().createSingletonTagCollection("form"))
    );
    LayerTagCollection tagParse = lattice.getLayerTagManager().createTagCollectionFromList(
        boost::assign::list_of("gobio")("parse")
    );

    std::vector<zvalue> results;
    BOOST_FOREACH(Edge e, choosen_edges) {
        zvalue zv = edge2zsyntree_(
            ch,
            e,
            local_rules,
            lang_spec,
            holder);
        results.push_back(zv);
    }

}


zvalue Gobio::edge2zsyntree_(
    Chart& chart,
    Edge edge,
    std::vector<Combinator::rule_holder>& local_rules,
    LangSpec lang_spec,
    zobjects_holder* holder)
{
    std::pair<
    t5_chart_type::variant_iterator,
    t5_chart_type::variant_iterator> vits
    = chart.edge_variants(edge);

    if(vits.first == vits.second)
    return NULL_ZVALUE;

    t5_chart_type::partition_iterator pit = chart.variant_partition(vits.first);

    boost::shared_ptr<tree_specification<int> > core_spec
    = combinator_.tree_spec(chart.partition_rule(pit), local_rules);

    assert(core_spec);

    return edge2zsyntree_with_spec_(
    chart,
    edge,
    vits.first,
    local_rules,
    core_spec,
    true,
    lang_spec,
    holder);
}


zvalue Gobio::edge2zsyntree_with_spec_(
    Chart& chart,
    Edge edge,
    Chart::variant_iterator vit,
    std::vector<Combinator::rule_holder>& local_rules,
    boost::shared_ptr<tree_specification<int> > spec,
    bool is_main,
    LangSpec lang_spec,
    zobjects_holder* holder)
{
std::cerr << "edge2zsyntree_with_spec_" << std::endl;
    zsyntree* R = zsyntree::generate(holder);

    boost::shared_ptr<tree_branch<int,t5_chart_type,t5_combinator_type::equivalent_type> > tb =
    extract_tree_branch_with_spec<int,t5_chart_type,t5_combinator_type>(
        chart,
        edge,
        vit,
        combinator_,
        local_rules,
        spec,
        is_main
        );

    R->setCategory(
    sym_fac_->get_symbol(
        combinator_.get_master().string_representation(tb->root()).c_str()));

    if(tb->is_supported())
    {
    int def = combinator_.get_master().false_value();

    {
        const av_matrix<int, int>& avm = chart.edge_category(tb->supporting_edge());

        for(int ai = 0; ai < avm.nb_attrs(); ++ai)
        if(combinator_.get_master().is_true(avm.get_attr(ai, def)))
            R->setAttr(
            sym_fac_->get_symbol(
                combinator_.get_attribute_registrar().get_obj(ai).c_str()),
            t5_value_to_zvalue_(
                combinator_.get_master(),
                avm.get_attr(ai, def)));
    }

    {
        const bare_av_matrix<int>::type& v_avm
        = chart.edge_variant_category(tb->supporting_variant());

        for(int v_ai = 0; v_ai < v_avm.nb_attrs(); ++v_ai)
        if(combinator_.get_master().is_true(v_avm.get_attr(v_ai, def)))
        {
            zsymbol* aname_sm =
            sym_fac_->get_symbol(
                combinator_.get_extra_attribute_registrar().get_obj(v_ai).c_str());

            if(combinator_.get_master().is_any(
               v_avm.get_attr(v_ai, def))
               &&
               (aname_sm == sm_S ||
            aname_sm == sm_Sem1 ||
            aname_sm == sm_Sem2 ||
            aname_sm == sm_Sem3))
            R->setAttr(
                aname_sm,
                sm_any);
            else
            R->setAttr(
                aname_sm,
                t5_value_to_zvalue_(
                combinator_.get_master(),
                v_avm.get_attr(v_ai, def)));
        }
    }
    }

    for(size_t i = 0; i < tb->nb_children(); ++i)
    {
    zvalue sub_zv =
        edge2zsyntree_with_spec_(
        chart,
        tb->child_edge(i),
        tb->child_variant_it(i),
        local_rules,
        tb->child_spec(i),
        false,
        lang_spec,
        holder);

    if(ZSYNTREEP(sub_zv))
        R->addSubtree(
        ZSYNTREEC(sub_zv),
        (combinator_.get_master().is_true(tb->child_label(i))
         ? sym_fac_->get_symbol(
             combinator_.get_master().string_representation(tb->child_label(i)).c_str())
         : NULL));
    }

    if(tb->equivalent())
    {
    R->setEquivTree(
        get_equiv_tree_(
        tb->equivalent(),
        lang_spec,
        holder));

    }

    return R;
}


void Gobio::markTree_(
    Lattice & lattice,
    LayerTagMask sourceMask,
    LayerTagCollection targetTags,
    Lattice::EdgeDescriptor edge
) {
    if (matches(lattice.getEdgeLayerTags(edge), sourceMask)) {
        const std::list<Lattice::Partition> partitions = lattice.getEdgePartitions(edge);
        Lattice::EdgeSequence edgeSequence;
        if (!partitions.empty()) {
            Lattice::Partition bestPartition = partitions.front();
            BOOST_FOREACH(Lattice::Partition partition, partitions) {
                if (partition.getScore() > bestPartition.getScore()) {
                    bestPartition = partition;
                }
            }
            Lattice::Partition::Iterator pi(lattice, bestPartition);
            while (pi.hasNext()) {
                markTree_(lattice, sourceMask, targetTags, pi.next());
            }
            edgeSequence = bestPartition.getSequence();
        }
        lattice.addEdge(
            lattice.getEdgeSource(edge),
            lattice.getEdgeTarget(edge),
            lattice.getEdgeAnnotationItem(edge),
            targetTags,
            edgeSequence
        );
    }
}


double Gobio::doGetQualityScore(
    const boost::program_options::variables_map & /*options*/) const {

    return -9999;
}
