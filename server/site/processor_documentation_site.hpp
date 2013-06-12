#ifndef PROCESSOR_DOCUMENTATION_SITE_HDR
#define PROCESSOR_DOCUMENTATION_SITE_HDR

#include <string>

#include "template_site.hpp"

class ProcessorDocumentationSite : public TemplateSite
{

public:
    ProcessorDocumentationSite(PsiServer& server);

    char * processorName();
    char * processorDocumentation();
    char * actionProcessorDocumentation();

private:
    std::string name_;
    std::ostringstream documentation_;

    static const std::string PROCESSOR_NAME_PARAM;
};

#endif
