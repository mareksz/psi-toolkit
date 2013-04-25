#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

#include "shallow_aliaser.hpp"

#include <iostream>

#include "split_pipeline.hpp"


typedef std::map<std::string, std::string> string_map;

ShallowAliaser::ShallowAliaser() {
    initializeAliases_();
}

std::string ShallowAliaser::replace(const std::string& pipeline) {
    std::string result = std::string(" " + pipeline + " ");

    // FIXME: optimize using regexps
    BOOST_FOREACH(const string_map::value_type& pair, aliases_) {
        boost::replace_all(result, pair.first, pair.second);
    }

    boost::algorithm::trim(result);
    return result;
}

std::vector<std::string> ShallowAliaser::replace(std::vector<std::string> pipeline) {
    for (size_t i = 0; i < pipeline.size(); ++i) {
        if (pipeline[i].find(" ") != std::string::npos) {
            pipeline[i] = "\"" + pipeline[i] + "\"";
        }
    }

    std::string pipelineAsString = boost::algorithm::join(pipeline, " ");
    std::string resultAsString = replace(pipelineAsString);

    return splitPipeline(resultAsString);
}

void ShallowAliaser::initializeAliases_() {
    addReadReaderAlias("docx", "apertium-reader --format docx");
    addReadReaderAlias("xlsx", "apertium-reader --format xlsx");
    addReadReaderAlias("pptx", "apertium-reader --format pptx");
    addReadReaderAlias("html", "apertium-reader --format html");

    addWriteGetAlias("tokens",   "simple-writer --tags token");
    addWriteGetAlias("segments", "simple-writer --tags segment");

    addWriteWriterGetAlias("xml", "bracketing-writer --skip-symbol-edges --opening-bracket \n"
        "<edge tags=\"%T\" category=\"%c\" attrs=\"%A\"> --closing-bracket </edge>\n"
        " --av-separator : --av-pairs-separator ;");

    addWriteGetDrawAlias("tree", "gv-writer --tree");
    addWriteGetDrawAlias("parse-tree", "gv-writer --tree --tags parse");
}

void ShallowAliaser::addReadReaderAlias(std::string alias, std::string expand) {
    addAlias_("read-" + alias, expand);
    addAlias_(alias + "-reader", expand);
}

void ShallowAliaser::addWriteGetAlias(std::string alias, std::string expand) {
    addAlias_("write-" + alias, expand);
    addAlias_("get-" + alias, expand);
}

void ShallowAliaser::addWriteWriterGetAlias(std::string alias, std::string expand) {
    addWriteGetAlias(alias, expand);
    addAlias_(alias + "-writer", expand);
}

void ShallowAliaser::addWriteGetDrawAlias(std::string alias, std::string expand) {
    addWriteGetAlias(alias, expand);
    addAlias_("draw-" + alias, expand);
}

void ShallowAliaser::addAlias_(std::string key, std::string value) {
    aliases_.insert(std::pair<std::string, std::string>(" " + key + " ", " " + value + " "));
}
