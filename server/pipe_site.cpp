#include "pipe_site.hpp"
#include "pipe_runner.hpp"
#include "logging.hpp"
#include "session_manager.hpp"

#include <iostream>
#include <sstream>

#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>

const std::string PipeSite::initialText = "Ala ma kota.";
const std::string PipeSite::initialPipe = "tp-tokenizer --lang pl";

PipeSite::PipeSite(PsiServer& server)
    : TemplateSite(server)
{
    psiServer_.registerIncludeCode(
        "pipe_site_input_text", boost::bind(&PipeSite::inputText, this));

    psiServer_.registerIncludeCode(
        "pipe_site_pipe_text", boost::bind(&PipeSite::pipeText, this));

    psiServer_.registerActionCode(
        "pipe", boost::bind(&PipeSite::actionPipe, this));

    psiServer_.registerIncludeCode(
        "pipe_site_output_text", boost::bind(&PipeSite::outputText, this));

    psiServer_.registerIncludeCode(
        "pipe_site_options", boost::bind(&PipeSite::hiddenOptions, this));
}

char * PipeSite::inputText() {
    std::string input = getOrSetDefaultData("input-text", initialText);
    return stringToChar(input);
}

char * PipeSite::pipeText() {
    std::string pipe = getOrSetDefaultData("pipe-text", initialPipe);
    return stringToChar(pipe);
}

char * PipeSite::outputText() {
    std::string output = getOrSetDefaultData("output-text", runPipe());
    output = std::string("<pre>") + output + std::string("</pre>");

    return stringToChar(output);
}

char * PipeSite::actionPipe() {
    //FIXME:
    //check if pipe and/or input are empty, if true then set default values and
    //show appropriate messages
    std::string pipe = psiServer_.session()->getData("pipe-text");

    std::string input = getInput();
    psiServer_.session()->setData("input-text", input);

    std::string output = runPipe();
    psiServer_.session()->setData("output-text", output);

    return stringToChar(std::string("/index.html"));
}

char * PipeSite::hiddenOptions() {
    /*
    std::string is_input_file_on = fileName.empty() ? "" : "on";
    std::string opts =
        std::string("<div input_file=\"") + is_input_file_on
        + std::string("\" />");
    */
    return stringToChar(std::string(" "));
}

std::string PipeSite::getOrSetDefaultData(const char* name, std::string initialValue) {
    if (!psiServer_.session()->isData(name)) {
        psiServer_.session()->setData(name, initialValue);
    }
    return psiServer_.session()->getData(name);
}

std::string PipeSite::getInput() {
    std::string input = psiServer_.session()->getData("input-text");

    //fileName = "";
/*
    std::string isFile = psiServer_.session()->getData("radio-file");
    if (isFile == "on") {
        //fileName = psiServer_.findValue("input-file-filename");
        in = psiServer_.findValue("input-file");
    }
*/
    if (input.empty())
        input = initialText;

    return input;
}

std::string PipeSite::runPipe() {
    std::string input = psiServer_.session()->getData("input-text");
    std::string pipe = psiServer_.session()->getData("pipe-text");

    if (input.empty())
        input = initialText;

    std::stringstream iss(input);
    std::ostringstream oss;

    INFO("Constructing pipe [" << pipe << "]...");
    INFO("Input is: " << input);

    try {
        PipeRunner p(pipe);
        INFO("... running");
        p.run(iss, oss);
        INFO("... OK");
    }
    catch(std::exception& e) {
        oss << "There are some problems: " << e.what() << std::endl
            << "Check the pipe-line specification and try once again.";
    }

    return oss.str();
}

