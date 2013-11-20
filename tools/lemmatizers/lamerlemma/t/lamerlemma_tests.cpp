#include "tests.hpp"

#include "lamerlemma.hpp"
#include "by_spaces_cutter.hpp"

#include "lemmatizer_annotator.hpp"

BOOST_AUTO_TEST_SUITE( lamerlemma )

BOOST_AUTO_TEST_CASE( lexeme_level ) {

    AnnotationItemManager aim;
    Lattice lattice(aim);
    std::string paragraph = "prowokacjami";
    lattice.appendStringWithSymbols(paragraph);

    BySpacesCutter cutter;
    LayerTagMask symbolMask = lattice.getLayerTagManager().getSingletonMask("symbol");
    LayerTagMask textMask = lattice.getLayerTagManager().getSingletonMask("text");

    LayerTagCollection textTags(
        lattice.getLayerTagManager().createSingletonTagCollection("text"));
    AnnotationItem item("TEXT", paragraph);
    lattice.addEdge(lattice.getFirstVertex(), lattice.getLastVertex(),
                    item, textTags);

    lattice.runCutter(cutter, symbolMask, textMask);

    boost::program_options::variables_map noOptions;
    LemmatizerAnnotator<LamerLemma> annotator(noOptions);
    annotator.annotate(lattice);

    // now checking
    {
        LayerTagMask lemmaMask_ = lattice.getLayerTagManager().getSingletonMask("lexeme");
        Lattice::EdgesSortedByTargetIterator lemmaIter = lattice.edgesSortedByTarget(lemmaMask_);

        BOOST_CHECK(lemmaIter.hasNext());

        Lattice::EdgeDescriptor prowokacjamiLemma = lemmaIter.next();
        AnnotationItem prowokacjamiItem = lattice.getEdgeAnnotationItem(prowokacjamiLemma);

        BOOST_CHECK_EQUAL(prowokacjamiItem.getCategory(), "subst");
        BOOST_CHECK_EQUAL(prowokacjamiItem.getText(), "prowokacja+subst");

        std::list<std::pair<std::string, std::string> > lexemeValues =
            aim.getValues(prowokacjamiItem);

        BOOST_CHECK_EQUAL((int)lexemeValues.size(), 1);
        std::list<std::pair<std::string, std::string> >::iterator valItr = lexemeValues.begin();

        BOOST_CHECK_EQUAL(valItr->first, "gender");
        BOOST_CHECK_EQUAL(valItr->second, "f");

        BOOST_CHECK(!lemmaIter.hasNext());
    }

    {
        LayerTagMask formMask_ = lattice.getLayerTagManager().getSingletonMask("form");
        Lattice::EdgesSortedByTargetIterator formIter = lattice.edgesSortedByTarget(formMask_);

        BOOST_CHECK(formIter.hasNext());

        Lattice::EdgeDescriptor prowokacjamiForm = formIter.next();
        AnnotationItem prowokacjamiItem = lattice.getEdgeAnnotationItem(prowokacjamiForm);

        BOOST_CHECK_EQUAL(prowokacjamiItem.getCategory(), "subst");
        BOOST_CHECK_EQUAL(prowokacjamiItem.getText(), "prowokacjami");

        std::list<std::pair<std::string, std::string> > formValues =
            aim.getValues(prowokacjamiItem);

        BOOST_CHECK_EQUAL((int)formValues.size(), 3);
        std::list<std::pair<std::string, std::string> >::iterator valItr = formValues.begin();

        BOOST_CHECK_EQUAL(valItr->first, "gender");
        BOOST_CHECK_EQUAL(valItr->second, "f");
        ++valItr;
        BOOST_CHECK_EQUAL(valItr->first, "case");
        BOOST_CHECK_EQUAL(valItr->second, "inst");
        ++valItr;
        BOOST_CHECK_EQUAL(valItr->first, "number");
        BOOST_CHECK_EQUAL(valItr->second, "pl");

        BOOST_CHECK(!formIter.hasNext());
    }
}

BOOST_AUTO_TEST_CASE( many_lexemes ) {

    AnnotationItemManager aim;
    Lattice lattice(aim);
    std::string paragraph = "mam";
    lattice.appendStringWithSymbols(paragraph);

    BySpacesCutter cutter;
    LayerTagMask symbolMask = lattice.getLayerTagManager().getSingletonMask("symbol");
    LayerTagMask textMask = lattice.getLayerTagManager().getSingletonMask("text");

    LayerTagCollection textTags(
        lattice.getLayerTagManager().createSingletonTagCollection("text"));
    AnnotationItem item("TEXT", paragraph);
    lattice.addEdge(lattice.getFirstVertex(), lattice.getLastVertex(),
                    item, textTags);

    lattice.runCutter(cutter, symbolMask, textMask);

    boost::program_options::variables_map noOptions;
    LemmatizerAnnotator<LamerLemma> annotator(noOptions);
    annotator.annotate(lattice);

    // now checking
    {
        LayerTagMask lemmaMask_ = lattice.getLayerTagManager().getSingletonMask("lexeme");
        Lattice::EdgesSortedByTargetIterator lemmaIter = lattice.edgesSortedByTarget(lemmaMask_);

        BOOST_CHECK(lemmaIter.hasNext());

        Lattice::EdgeDescriptor lemma = lemmaIter.next();
        AnnotationItem item = lattice.getEdgeAnnotationItem(lemma);

        BOOST_CHECK_EQUAL(item.getCategory(), "subst");
        BOOST_CHECK_EQUAL(item.getText(), "mama+subst");

        BOOST_CHECK(lemmaIter.hasNext());

        lemma = lemmaIter.next();
        item = lattice.getEdgeAnnotationItem(lemma);

        BOOST_CHECK_EQUAL(item.getCategory(), "verb");
        BOOST_CHECK_EQUAL(item.getText(), "mamić+verb");
        BOOST_CHECK(lemmaIter.hasNext());

        lemma = lemmaIter.next();
        item = lattice.getEdgeAnnotationItem(lemma);

        BOOST_CHECK_EQUAL(item.getCategory(), "verb");
        BOOST_CHECK_EQUAL(item.getText(), "mieć+verb");

        BOOST_CHECK(!lemmaIter.hasNext());
    }
}

BOOST_AUTO_TEST_SUITE_END()
