#ifndef HELP_SITE_HDR
#define HELP_SITE_HDR

#include "help_template_site.hpp"
#include "html_help_formatter.hpp"
#include "file_storage.hpp"

#include <boost/program_options/options_description.hpp>

class HelpSite : public HelpTemplateSite {

public:

    HelpSite(PsiServer& servers);

    char * description();
    char * processorHelps();
    char * aliases();
    char * tutorial();
    char * licence();
    char * documentationMenu();
    char * psiFormat();
    char * faq();
    char * installationGuide();

    char * pipelineExamples();

};

#endif
