#ifndef PIPE_SITE_HDR
#define PIPE_SITE_HDR

#include <string>

#include "template_site.hpp"
#include "file_storage.hpp"
#include "file_recognizer.hpp"
#include "encoding_converter.hpp"


class PipeSite : public TemplateSite
{

public:

    PipeSite(PsiServer& server, const std::string & pipe, const std::string & text);

    char * inputText();
    char * pipeText();
    char * actionPipe();
    char * outputText();

    char * hiddenOptions();
private:

    void registerIncludesAndActions_();

    const std::string initialText_;
    const std::string initialPipe_;
    std::string initialOutput_;

    std::string getOrSetDefaultData_(const char* name, std::string initialValue);

    std::string getInput_();
    std::string getPipe_();
    bool inputFromFile_;
    std::string runPipe_(std::string pipe, std::string input, bool withSession = true);

    FileStorage fileStorage_;
    FileRecognizer fileRecognizer_;

    void preparePipelineAndInput_(std::string& pipe, std::string& input);

    void clearPreviousFileFromOutput_();
    void createFileFromOutput_(const std::string& output);
    std::string generateOutput_(const std::string& rawOuput);

    static const std::string GUESSING_READER;
    void tryToAddGuessingReader_(std::string& pipe);

    std::string encodeHTML_(const std::string& data);

    EncodingConverter encodingConverter_;
};

#endif
