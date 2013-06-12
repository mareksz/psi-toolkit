#include <string>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#include "processor_documentation_site.hpp"
#include "pipe_runner.hpp"
#include "session_manager.hpp"

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
    }
    else {
        documentation_ << "Page does not exist!" << std::endl;
    }

    documentation_ << name_ << std::endl;

    return stringToChar(std::string("/help/processor-documentation.html"));
}
