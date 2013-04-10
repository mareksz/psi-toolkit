#ifndef ENCODING_CONVERTER_HDR
#define ENCODING_CONVERTER_HDR

#include <string>
#include <map>

class EncodingConverter {
public:

    EncodingConverter();
    EncodingConverter(std::string targetEncoding);

    std::string detect(std::string text);

    std::string convert(std::string from, std::string to, std::string text);
    std::string convert(std::string from, std::string text);
    std::string convert(std::string text);

    std::string getTargetEncoding();

    static std::vector<std::string> getAllEncodings();
    static const std::string ASCII;

private:

    std::string targetEncoding_;

    bool convert_(int inCharsetId, int outCharsetId, std::string input, std::string& output);
    bool detect_(const char* input, size_t length, std::string& encoding);

    static std::map<std::string, int> CHARSET_CODES;
    static const int TINICONV_OPTION;
    static const int BUFFER_SIZE;
};

#endif
