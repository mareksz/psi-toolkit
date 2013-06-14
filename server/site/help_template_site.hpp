#ifndef HELP_TEMPLATE_SITE_HDR
#define HELP_TEMPLATE_SITE_HDR

#include "template_site.hpp"
#include "html_help_formatter.hpp"
#include "file_storage.hpp"

class HelpTemplateSite : public TemplateSite {

public:

    HelpTemplateSite(PsiServer& server)
        : TemplateSite(server),
        fileStorage_(std::string(psiServer_.websiteRoot)),
        htmlHelpFormatter_()
    {
        htmlHelpFormatter_.setFileStorage(&fileStorage_);
    }

protected:

    FileStorage fileStorage_;
    HtmlHelpFormatter htmlHelpFormatter_;

};

#endif
