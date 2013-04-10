#include "tests.hpp"
#include "config.hpp"

#include <iostream>
#include <fstream>

#include "encoding_converter.hpp"

BOOST_AUTO_TEST_SUITE( encoding_converter )

std::string getContentOfExampleFile_(std::string fileName);

BOOST_AUTO_TEST_CASE( tiniconv_conversion ) {
    EncodingConverter converter;

    // uchardet detects below example as text encoded in windows-1252 instead of ISO-8859-2,
    std::string input = getContentOfExampleFile_("example_iso-8859-2.txt");
    std::string result = converter.convert("ISO-8859-2", "UTF-8", input);

    BOOST_CHECK_EQUAL("Zażółć gęślą jaźń.", result);

    input = getContentOfExampleFile_("example_windows-1255.txt");
    result = converter.convert("windows-1255", "UTF-8", input);

    BOOST_CHECK_EQUAL("למההםפשוטלאמדבריםעברי", result);

    input = getContentOfExampleFile_("example_windows-1251.txt");
    result = converter.convert("windows-1251", "UTF-8", input);

    BOOST_CHECK_EQUAL("Этот только UTF, да и вообще перевод не корректный", result);
}

BOOST_AUTO_TEST_CASE( uchardet_detection ) {
    EncodingConverter converter;

    std::string input = getContentOfExampleFile_("example_windows-1255.txt");
    std::string result = converter.convert("windows-1255", "UTF-8", input);

    BOOST_CHECK_EQUAL("למההםפשוטלאמדבריםעברי", result);

    input = getContentOfExampleFile_("example_windows-1251.txt");
    result = converter.convert("windows-1251", "UTF-8", input);

    BOOST_CHECK_EQUAL("Этот только UTF, да и вообще перевод не корректный", result);
}

BOOST_AUTO_TEST_CASE( conversion_with_default_target_encoding ) {
    EncodingConverter converter;

    std::string input = getContentOfExampleFile_("example_windows-1255.txt");
    std::string result = converter.convert("windows-1255", input);

    BOOST_CHECK_EQUAL("למההםפשוטלאמדבריםעברי", result);

    input = getContentOfExampleFile_("example_iso-8859-2.txt");
    result = converter.convert("ISO-8859-2", input);

    BOOST_CHECK_EQUAL("Zażółć gęślą jaźń.", result);
}

BOOST_AUTO_TEST_CASE( auto_conversion ) {
    EncodingConverter converter;

    std::string input = getContentOfExampleFile_("example_windows-1255.txt");
    std::string result = converter.convert(input);

    BOOST_CHECK_EQUAL("למההםפשוטלאמדבריםעברי", result);
}

BOOST_AUTO_TEST_CASE( specific_causes ) {
    EncodingConverter converter;

    std::string input = "text in ascii which shouldn't be converted";

    std::string encoding = converter.detect(input);
    BOOST_CHECK_EQUAL(EncodingConverter::ASCII, encoding);

    std::string output = converter.convert(input);
    BOOST_CHECK_EQUAL(input, output);

    input = "przykładowy tekst w UTF-8 (zażółć gęślą jaźń)";

    output = converter.convert("UTF-8", "UTF-8", input);
    BOOST_CHECK_EQUAL(input, output);
}

std::string getContentOfExampleFile_(std::string fileName) {
    std::ifstream file(std::string(ROOT_DIR "common/t/" + fileName).c_str());

    std::string text, line;
    while (std::getline(file, line)) text += line;

    file.close();
    return text;
}

BOOST_AUTO_TEST_SUITE_END()
