#include <string>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#include "processor_documentation_site.hpp"
#include "pipe_runner.hpp"
#include "session_manager.hpp"
#include "html_help_formatter.hpp"

const std::string ProcessorDocumentationSite::PROCESSOR_NAME_PARAM = "name";

ProcessorDocumentationSite::ProcessorDocumentationSite(PsiServer& server)
    : TemplateSite(server),
    name_(pageNotFoundMessage()),
    documentation_(""),
    fileStorage_(std::string(psiServer_.websiteRoot)),
    htmlHelpFormatter_()
{
    htmlHelpFormatter_.setFileStorage(&fileStorage_);

    psiServer_.registerIncludeCode("processor_documentation_site_processor_name",
        boost::bind(&ProcessorDocumentationSite::processorName, this));
    psiServer_.registerIncludeCode("processor_documentation_site_processor_documentation",
        boost::bind(&ProcessorDocumentationSite::processorDocumentation, this));

    psiServer_.registerActionCode("help/documentation/processor",
        boost::bind(&ProcessorDocumentationSite::actionProcessorDocumentation, this));
}

char * ProcessorDocumentationSite::processorName() {
    return stringToChar(name_);
}

char * ProcessorDocumentationSite::processorDocumentation() {
    if (documentation_.str().empty()) {
        documentation_ << pageNotFoundMessage();
    }
    return stringToChar(documentation_.str());
}

char * ProcessorDocumentationSite::actionProcessorDocumentation() {
    documentation_.str(std::string());

    if (psiServer_.session()->isData(PROCESSOR_NAME_PARAM)) {
        name_ = psiServer_.session()->getData(PROCESSOR_NAME_PARAM);

        if (not htmlHelpFormatter_.formatProcessorHelpsByName(name_, documentation_)) {
            documentation_ << "Processor <code>" << name_ << "</code> not found." << std::endl;
            name_ = pageNotFoundMessage();
        }
    }
    else {
        name_ = pageNotFoundMessage();
        documentation_ << "Processor not specified.";
    }

    psiServer_.session()->clearData(PROCESSOR_NAME_PARAM);
    return stringToChar(std::string("/help/processor-documentation.html"));
}
