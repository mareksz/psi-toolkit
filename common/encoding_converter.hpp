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

        static std::map<std::string, int> CHARSET_CODES;
};

#endif
