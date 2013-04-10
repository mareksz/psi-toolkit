#ifndef ENCODING_CONVERTER_HDR
#define ENCODING_CONVERTER_HDR

#include <string>
#include <map>

class EncodingConverter {
    public:

        EncodingConverter();
        EncodingConverter(std::string defaultEncoding);

        std::string detect(std::string text);

        std::string convert(std::string from, std::string to, std::string text);
        //std::string convert(std::string to, std::string text);
        //std::string convert(std::string text)

    private:

        std::string defaultEncoding_;

        bool convert_(int inCharsetId, int outCharsetId, std::string input, std::string& output);
        bool detect_(const char* input, size_t length, std::string& output);

        static std::map<std::string, int> CHARSET_CODES;
        static const int TINICONV_OPTION;
        static const int BUFFER_SIZE;
};

#endif
