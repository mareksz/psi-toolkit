#include "tests.hpp"

#include "annotation_item_manager.hpp"

BOOST_AUTO_TEST_CASE( annotation_simple ) {
    AnnotationItemManager manager;
    AnnotationItem annotationItem("noun");
    BOOST_CHECK_EQUAL(manager.getCategory(annotationItem), annotationItem.getCategory());
    BOOST_CHECK_EQUAL(manager.getCategory(annotationItem), "noun");
    BOOST_CHECK_EQUAL(manager.getValue(annotationItem, "case"), NULL_ZVALUE);
    BOOST_CHECK_EQUAL(manager.getValueAsString(annotationItem, "case"), "fail");
    manager.setValue(annotationItem, "case", "nominative");
    BOOST_CHECK_EQUAL(manager.getValueAsString(annotationItem, "case"), "nominative");
    manager.setValue(annotationItem, "case", "genitive");
    BOOST_CHECK_EQUAL(manager.getValueAsString(annotationItem, "case"), "genitive");
    manager.setValue(annotationItem, "number", "plural");
    BOOST_CHECK_EQUAL(manager.getValueAsString(annotationItem, "number"), "plural");
    manager.setValue(annotationItem, "count", INTEGER_TO_ZVALUE(123));
    BOOST_CHECK_EQUAL(ZVALUE_TO_INTEGER(manager.getValue(annotationItem, "count")), 123);

    std::list< std::pair<std::string, std::string> > av = manager.getValues(annotationItem);
    std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "case");
    BOOST_CHECK_EQUAL((*avi).second, "genitive");
    ++avi;
    BOOST_REQUIRE(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "plural");

    manager.setCategory(annotationItem, "pronoun");
    BOOST_CHECK_EQUAL(manager.getCategory(annotationItem), annotationItem.getCategory());
    BOOST_CHECK_EQUAL(manager.getCategory(annotationItem), "pronoun");
    BOOST_CHECK_EQUAL(manager.getValueAsString(annotationItem, "case"), "genitive");
    BOOST_CHECK_EQUAL(manager.getValueAsString(annotationItem, "number"), "plural");
};

BOOST_AUTO_TEST_CASE( annotation_special ) {
    AnnotationItemManager manager;
    AnnotationItem annotationItem("verb");
    manager.setValue(annotationItem, "object", "fail");
    BOOST_CHECK_EQUAL(manager.getValue(annotationItem, "object"), NULL_ZVALUE);
    BOOST_CHECK_EQUAL(manager.getValueAsString(annotationItem, "object"), "fail");
    manager.setValue(annotationItem, "object", "false");
    BOOST_CHECK_EQUAL(manager.getValue(annotationItem, "object"), NULL_ZVALUE);
    BOOST_CHECK_EQUAL(manager.getValueAsString(annotationItem, "object"), "fail");
    manager.setValue(annotationItem, "object", "empty");
    BOOST_CHECK_EQUAL(manager.getValue(annotationItem, "object"), NULL_ZVALUE);
    BOOST_CHECK_EQUAL(manager.getValueAsString(annotationItem, "object"), "fail");
    manager.setValue(annotationItem, "object", "nil");
    BOOST_CHECK_EQUAL(manager.getValue(annotationItem, "object"), DEFAULT_ZVALUE);
    BOOST_CHECK_EQUAL(manager.getValueAsString(annotationItem, "object"), "nil");
    manager.setValue(annotationItem, "object", "any");
    BOOST_CHECK_EQUAL(manager.getValue(annotationItem, "object"), DEFAULT_ZVALUE);
    BOOST_CHECK_EQUAL(manager.getValueAsString(annotationItem, "object"), "nil");
}

BOOST_AUTO_TEST_CASE( annotation_more ) {
    AnnotationItemManager manager;
    AnnotationItem annotationItem1("noun");
    manager.setValue(annotationItem1, "case", "nominative");
    AnnotationItem annotationItem2("verb");
    manager.setValue(annotationItem2, "tense", "past");
    BOOST_CHECK_EQUAL(manager.getValueAsString(annotationItem1, "case"), "nominative");
    BOOST_CHECK_EQUAL(manager.getValue(annotationItem1, "tense"), NULL_ZVALUE);
    BOOST_CHECK_EQUAL(manager.getValue(annotationItem2, "case"), NULL_ZVALUE);
    BOOST_CHECK_EQUAL(manager.getValueAsString(annotationItem2, "tense"), "past");
}

BOOST_AUTO_TEST_CASE( annotation_compare ) {
    AnnotationItemManager manager;
    AnnotationItem ai1("noun");
    AnnotationItem ai2("noun");
    manager.setValue(ai1, "case", "nominative");
    manager.setValue(ai1, "number", "plural");
    manager.setValue(ai2, "number", "plural");
    manager.setValue(ai2, "case", "nominative");
    BOOST_CHECK(ai1 == ai2);
};

BOOST_AUTO_TEST_CASE( annotation_copy ) {
    AnnotationItem ai("noun", StringFrag("ala"));
    AnnotationItem ai_c(ai, "verb");
    AnnotationItem ai_t(ai, StringFrag("ola"));
    BOOST_CHECK_EQUAL(ai.getCategory(), "noun");
    BOOST_CHECK_EQUAL(ai.getText(), "ala");
    BOOST_CHECK_EQUAL(ai_c.getCategory(), "verb");
    BOOST_CHECK_EQUAL(ai_c.getText(), "ala");
    BOOST_CHECK_EQUAL(ai_t.getCategory(), "noun");
    BOOST_CHECK_EQUAL(ai_t.getText(), "ola");
};

BOOST_AUTO_TEST_CASE( annotation_text ) {
    AnnotationItemManager manager;
    AnnotationItem ai("noun", StringFrag("ala"));
    manager.setValue(ai, "case", "nominative");
    manager.setValue(ai, "number", "plural");
    BOOST_CHECK_EQUAL(ai.getCategory(), "noun");
    BOOST_CHECK_EQUAL(ai.getText(), "ala");
};

BOOST_AUTO_TEST_CASE( annotation_text_as_string_frag ) {
    AnnotationItemManager manager;
    std::string text("Ala ma kota.");
    AnnotationItem ai("noun", StringFrag(text, 4, 2));
    manager.setValue(ai, "case", "nominative");
    manager.setValue(ai, "number", "plural");
    BOOST_CHECK_EQUAL(ai.getCategory(), "noun");
    BOOST_CHECK_EQUAL(ai.getText(), "ma");
    BOOST_CHECK_EQUAL(ai.getTextAsStringFrag().str(), "ma");
};

BOOST_AUTO_TEST_CASE( zvalue_conversion_text_converter ) {
    AnnotationItemManager manager;
    std::string text("Ala ma kota.");
    zvalue ztext = manager.stringToZvalue(text);
    std::string text2 = manager.zvalueToString(ztext);
    BOOST_CHECK_EQUAL(text2, text);
    zvalue ztext2 = manager.stringToZvalue(text2);
    BOOST_CHECK_EQUAL(ztext2, ztext);
};

BOOST_AUTO_TEST_CASE( zvalue_conversion_text_master ) {
    AnnotationItemManager manager;
    std::string text("Ala ma kota.");
    zvalue ztext = manager.from_string(text);
    std::string text2 = manager.to_string(ztext);
    BOOST_CHECK_EQUAL(text2, text);
    zvalue ztext2 = manager.from_string(text2);
    BOOST_CHECK_EQUAL(ztext2, ztext);
};

BOOST_AUTO_TEST_CASE( zvalue_conversion_special ) {
    AnnotationItemManager manager;
    zvalue zany = manager.any_value();
    zvalue zfalse = manager.false_value();
    std::string sany = manager.to_string(zany);
    std::string sfalse = manager.to_string(zfalse);
    zvalue zany2 = manager.from_string(sany);
    zvalue zfalse2 = manager.from_string(sfalse);
    BOOST_CHECK_EQUAL(zany, zany2);
    BOOST_CHECK_EQUAL(zfalse, zfalse2);
};

BOOST_AUTO_TEST_CASE( zvalue_conversion_bool ) {
    AnnotationItemManager manager;
    zvalue ztrue = manager.from_bool(true);
    zvalue zfalse = manager.from_bool(false);
    std::string strue = manager.to_string(ztrue);
    std::string sfalse = manager.to_string(zfalse);
    zvalue ztrue2 = manager.from_string(strue);
    zvalue zfalse2 = manager.from_string(sfalse);
    BOOST_CHECK_EQUAL(ztrue, ztrue2);
    BOOST_CHECK_EQUAL(zfalse, zfalse2);
};

BOOST_AUTO_TEST_CASE( zvalue_conversion_number ) {
    AnnotationItemManager manager;
    int inumber = 123;
    zvalue znumber = manager.from_int(inumber);
    int inumber2 = manager.to_int(znumber);
    BOOST_CHECK_EQUAL(inumber, inumber2);

    std::string snumber = manager.to_string(znumber);
    std::stringstream ssnumber(snumber);
    int inumber3;
    ssnumber >> inumber3;
    BOOST_CHECK_EQUAL(inumber, inumber3);
};
