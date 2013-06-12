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
    name_("unknown"),
    documentation_("Page not found!")
{
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
        documentation_ << "Page not found!";
    }
    return stringToChar(documentation_.str());
}

char * ProcessorDocumentationSite::actionProcessorDocumentation() {
    documentation_.str(std::string());

    if (psiServer_.session()->isData(PROCESSOR_NAME_PARAM)) {
        name_ = psiServer_.session()->getData(PROCESSOR_NAME_PARAM);

        if (not HtmlHelpFormatter().formatProcessorHelpsByName(name_, documentation_)) {
            name_ = "page not found";
            documentation_ << "Processor(s) <code>" << name_ << "</code> not found." << std::endl;
        }
    }
    else {
        documentation_ << "Processor(s) not specified.";
    }

    psiServer_.session()->clearData(PROCESSOR_NAME_PARAM);
    return stringToChar(std::string("/help/processor-documentation.html"));
}
