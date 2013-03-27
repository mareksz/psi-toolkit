#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

#include "shallow_aliaser.hpp"
#include <iostream>


typedef std::map<std::string, std::string> string_map;

const std::map<std::string, std::string> ShallowAliaser::aliases_ =
    boost::assign::map_list_of
        ("write-tokens",    "simple-writer --tag token")
        ("get-tokens",      "simple-writer --tag token")
        ("write-segments",  "simple-writer --tag segment")
        ("get-segments",    "simple-writer --tag segment")

        ("read-docx",       "apertium-reader --format docx")
        ("docx-reader",     "apertium-reader --format docx")
        ("read-xlsx",       "apertium-reader --format xlsx")
        ("xlsx-reader",     "apertium-reader --format xlsx")
        ("read-pptx",       "apertium-reader --format pptx")
        ("pptx-reader",     "apertium-reader --format pptx")
        ("read-html",       "apertium-reader --format html")
        ("html-reader",     "apertium-reader --format html")

        //("write-tokens-and-segments",
        //    "write --tag symbol --sep '' --spec token ' ' segment \\n")
        //("get-tokens-and-segments",
        //    "write --tag symbol --sep '' --spec token ' ' segment \\n")

        ("tree",            "gv-writer --tree")
        ("write-tree",      "gv-writer --tree")
        ("draw-tree",       "gv-writer --tree")

        ("parse-tree",          "gv-writer --tree --filter parse")
        ("write-parse-tree",    "gv-writer --tree --filter parse")
        ("draw-parse-tree",     "gv-writer --tree --filter parse")

        ("write-xml",   "bracketing-writer --skip-symbol-edges --opening-bracket \n"
            "<edge tags=\"%T\" category=\"%c\" attrs=\"%A\"> --closing-bracket </edge>\n"
            " --av-separator : --av-pairs-separator ;")
        ("xml-writer",  "bracketing-writer --skip-symbol-edges --opening-bracket \n"
            "<edge tags=\"%T\" category=\"%c\" attrs=\"%A\"> --closing-bracket </edge>\n"
            " --av-separator : --av-pairs-separator ;")
        ;


std::string ShallowAliaser::replace(const std::string& pipeline) {
    std::string result = std::string(" " + pipeline + " ");

    // FIXME: optimize using regexps
    BOOST_FOREACH(const string_map::value_type& pair, aliases_) {
        boost::replace_all(result, " " + pair.first + " ", " " + pair.second + " ");
    }

    boost::algorithm::trim(result);
    return result;
}

std::vector<std::string> ShallowAliaser::replace(std::vector<std::string> pipeline) {
    std::vector<std::string> result;

    std::string pipelineAsString = boost::algorithm::join(pipeline, " ");
    std::string resultAsString = replace(pipelineAsString);

    boost::split(result, resultAsString, boost::is_any_of(" "));

    return result;
}

/*
void ShallowAliaser::initializeAliases_() {
    addWriteGetAlias("tokens",   "simple-writer --tag token");
    addWriteGetAlias("segments", "simple-writer --tag segment");

    addWriteGetAlias("tokens-and-segments",
        "write --tag symbol --sep '' --spec token ' ' segment \\n");
    addWriteGetAlias("tokens-with-segments",
        "write --tag symbol --sep '' --spec token ' ' segment \\n");

    addWriteGetAlias("xml", "bracketing-writer --skip-symbol-edges --opening-bracket \n"
        "<edge tags=\"%T\" category=\"%c\" attrs=\"%A\"> --closing-bracket </edge>\n"
        " --av-separator : --av-pairs-separator ;");

    addReadReaderAlias("docx", "apertium-reader --format docx");
    addReadReaderAlias("xlsx", "apertium-reader --format xlsx");
    addReadReaderAlias("pptx", "apertium-reader --format pptx");
    addReadReaderAlias("html", "apertium-reader --format html");

    addNullDrawWriteAlias("tree", "gv-writer --tree");
    addNullDrawWriteAlias("parse-tree", "gv-writer --tree --filter-parse");
}

void ShallowAliaser::addWriteGetAlias(std::string alias, std::string expand) {
    aliases_["write-" + alias] = expand;
    aliases_["get-" + alias] = expand;
}

void ShallowAliaser::addReadReaderAlias(std::string alias, std::string expand) {
    aliases_["read-" + alias] = expand;
    aliases_[alias + "-reader"] = expand;
}

void ShallowAliaser::addWriteWriterGetAlias(std::string alias, std::string expand) {
    addWriteGetAlias(alias, expand);
    aliases_[alias + "-writer"] = expand;
}

void ShallowAliaser::addNullDrawWriteAlias(std::string alias, std::string expand) {
    aliases_[alias] = expand;
    aliases_["draw-" + alias] = expand;
    aliases_["write-" + alias] = expand;
}
*/
