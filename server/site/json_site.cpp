#include <string>
#include <boost/bind.hpp>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "json_site.hpp"
#include "pipe_runner.hpp"
#include "session_manager.hpp"

const std::string JsonSite::PIPE_KEY_NAME = "pipe";
const std::string JsonSite::INPUT_KEY_NAME = "input";
const std::string JsonSite::OUTPUT_KEY_NAME = "output";
const std::string JsonSite::ERROR_KEY_NAME = "error";

std::map<std::string, std::string> JsonSite::ESCAPED_CHARS =
    boost::assign::map_list_of
        ("/", "\\/")
        ("\"", "\\\"")
        ("\n", "\\n")
        ("\r", "\\r")
        ("\t", "\\t")
        ("\b", "\\b")
        ;

const std::vector<std::string> JsonSite::JSON_WRITERS = boost::assign::list_of
    ("json-simple-writer");

JsonSite::JsonSite(PsiServer& server) : TemplateSite(server), output_("{}") {
    psiServer_.registerIncludeCode(
        "json_site_output", boost::bind(&JsonSite::jsonOutput, this));

    psiServer_.registerActionCode(
        "json", boost::bind(&JsonSite::actionJson, this));
}

char * JsonSite::jsonOutput() {
    return stringToChar(output_.str());
}

char * JsonSite::actionJson() {
    output_.str(std::string());
    output_ << "{ " << std::endl;

    std::string pipe = psiServer_.session()->getData(PIPE_KEY_NAME);
    addKeyValuePairToOutputAsString_(PIPE_KEY_NAME, escape_(pipe));

    std::string input = psiServer_.session()->getData(INPUT_KEY_NAME);
    addKeyValuePairToOutputAsString_(INPUT_KEY_NAME, escape_(input));

    std::stringstream iss(input);
    std::ostringstream oss;

    try {
        PipeRunner p(pipe);

        INFO("... running via API");
        p.run(iss, oss);
        INFO("... OK");

        if (isJavascriptOutput_(pipe)) {
            addKeyValuePairToOutput_(OUTPUT_KEY_NAME, oss.str());
        }
        else {
            addKeyValuePairToOutputAsString_(OUTPUT_KEY_NAME, oss.str());
        }
        addKeyValuePairToOutput_(ERROR_KEY_NAME, "null", true);
    }
    catch (std::exception& e) {
        oss << "There are some problems: " << e.what();

        addKeyValuePairToOutput_(OUTPUT_KEY_NAME, "null");
        addKeyValuePairToOutputAsString_(ERROR_KEY_NAME, oss.str(), true);
    }

    output_ << " }";

    return stringToChar(std::string("/json.html"));
}

void JsonSite::addKeyValuePairToOutput_(std::string key, std::string value, bool isLast) {
    output_ << "\"" << key << "\": " << value << (isLast ? "" : ",") << std::endl;
}

void JsonSite::addKeyValuePairToOutputAsString_(std::string key, std::string value, bool isLast) {
    output_ << "\"" << key << "\": \"" << value << "\"" << (isLast ? "" : ",") << std::endl;
}

bool JsonSite::isJavascriptOutput_(std::string pipe) {
    BOOST_FOREACH(std::string jsonWriter, JSON_WRITERS) {
        if (pipe.find(jsonWriter) != std::string::npos)
            return true;
    }
    return false;
}

std::string JsonSite::escape_(const std::string& text) {
    std::string escapedText(text);

    typedef std::map<std::string, std::string> string_map;
    BOOST_FOREACH(const string_map::value_type& pair, ESCAPED_CHARS) {
        boost::replace_all(escapedText, pair.first, pair.second);
    }

    return escapedText;
}
