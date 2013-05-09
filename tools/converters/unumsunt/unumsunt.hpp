#ifndef UNUMSUNT_HDR
#define UNUMSUNT_HDR


#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/spirit/include/qi.hpp>

#include "annotator.hpp"
#include "language_dependent_annotator_factory.hpp"
#include "lang_specific_processor_file_fetcher.hpp"
#include "unumsunt_rule.hpp"


namespace qi = boost::spirit::qi;


struct UnumsuntRuleItem {
    std::vector<std::string> conditions;
    std::vector<std::string> commands;
};


BOOST_FUSION_ADAPT_STRUCT(
    UnumsuntRuleItem,
    (std::vector<std::string>, conditions)
    (std::vector<std::string>, commands)
)


struct UnumsuntRuleGrammar : public qi::grammar<
    std::string::const_iterator,
    UnumsuntRuleItem()
> {

    UnumsuntRuleGrammar() : UnumsuntRuleGrammar::base_type(start) {

        start
            %= +(qi::char_ - ',' - '>') % ','
            >> qi::lit(">>")
            >> +(qi::char_ - ',') % ',';

    }

    qi::rule<std::string::const_iterator, UnumsuntRuleItem()> start;

};


struct UnumsuntAssignmentItem {
    std::string arg;
    std::string val;
};


BOOST_FUSION_ADAPT_STRUCT(
    UnumsuntAssignmentItem,
    (std::string, arg)
    (std::string, val)
)


struct UnumsuntAssignmentGrammar : public qi::grammar<
    std::string::const_iterator,
    UnumsuntAssignmentItem()
> {

    UnumsuntAssignmentGrammar() : UnumsuntAssignmentGrammar::base_type(start) {

        start
            %= +(qi::char_ - ',' - '=')
            >> '='
            >> +(qi::char_ - ',' - '=');

    }

    qi::rule<std::string::const_iterator, UnumsuntAssignmentItem()> start;

};


class Unumsunt : public Annotator {

public:

    class Factory : public LanguageDependentAnnotatorFactory {
        virtual Annotator* doCreateAnnotator(
            const boost::program_options::variables_map& options);

        virtual void doAddLanguageIndependentOptionsHandled(
            boost::program_options::options_description& optionsDescription);

        virtual std::string doGetName() const;
        virtual boost::filesystem::path doGetFile() const;

        virtual std::list<std::list<std::string> > doRequiredLayerTags();

        virtual std::list<std::list<std::string> > doOptionalLayerTags();

        virtual std::list<std::string> doProvidedLayerTags();

        static const std::string DEFAULT_RULE_FILE;
    };

    Unumsunt(std::string langCode, std::string rulesPath);

    void convertTags(Lattice & lattice);

private:

    class Worker : public LatticeWorker {
    public:
        Worker(Unumsunt& processor, Lattice& lattice);
    private:
        virtual void doRun();
        Unumsunt& processor_;
    };

    virtual LatticeWorker* doCreateLatticeWorker(Lattice& lattice);

    virtual std::string doInfo();

    std::string langCode_;

    std::string sourceTagset_;
    std::string targetTagset_;

    std::list<std::string> preservedTags_;

    std::map<std::string, std::string> cat_map_;
    std::map<std::string, std::string> attr_map_;
    std::map<std::string, std::string> val_map_;

    std::vector<UnumsuntRule> aux_rules_;

    std::map<Lattice::EdgeDescriptor, Lattice::EdgeDescriptor> replacements_;

};


#endif
