#include <boost/assign.hpp>

#include "encoding_converter.hpp"
#include "logging.hpp"

#include "tiniconv.h"
#include "uchardet.h"


const std::string EncodingConverter::ASCII_CHARSET = "ascii/unknown";

/*
 * Integers are set according to constant values from tiniconv library.
 * See file tiniconv/tiniconv.h
 */
std::map<std::string, int> EncodingConverter::CHARSET_CODES = boost::assign::map_list_of
    (ASCII_CHARSET,     0)
    ("windows-1250",    1)
    ("windows-1251",    2)
    ("windows-1252",    3)
    ("windows-1253",    4)
    ("windows-1254",    5)  // doesn't exist in uchardet
    ("windows-1255",    6)
    ("windows-1256",    7)
    ("windows-1257",    8)
    ("windows-1258",    9)
    ("CP939",           10)
    ("HZ-GB-2312",      11) // or EUC-CN like in tiniconv/tiniconv_desc.c?
    ("GBK",             12)
    ("ISO-2020-JP",     13)
    ("ISO-8859-1",      14) // doesn't exist in uchardet
    ("ISO-8859-2",      15)
    ("ISO-8859-3",      16) // doesn't exist in uchardet
    ("ISO-8859-4",      17) // doesn't exist in uchardet
    ("ISO-8859-5",      18)
    ("ISO-8859-6",      19) // doesn't exist in uchardet
    ("ISO-8859-7",      20)
    ("ISO-8859-8",      21)
    ("ISO-8859-9",      22) // doesn't exist in uchardet
    ("ISO-8859-10",     23) // doesn't exist in uchardet
    ("ISO-8859-11",     24) // doesn't exist in uchardet
    ("ISO-8859-13",     25) // doesn't exist in uchardet
    ("ISO-8859-14",     26) // doesn't exist in uchardet
    ("ISO-8859-15",     27) // doesn't exist in uchardet
    ("ISO-8859-16",     28) // doesn't exist in uchardet
    ("CP866",           29)
    ("KOI8-R",          30)
    ("KOI8-RU",         31) // doesn't exist in uchardet
    ("KOI8-U",          32) // doesn't exist in uchardet
    ("x-max-cyrillic",  33)
    ("UCS-2",           34) // doesn't exist in uchardet
    ("UTF-7",           35) // doesn't exist in uchardet
    ("UTF-8",           36)
    ("GB-2312",         37) // or EUC-CN like 11?
    ("Big5",            38)
    ;

std::vector<std::string> EncodingConverter::getAllEncodings() {
    std::vector<std::string> keys;

    for (std::map<std::string, int>::iterator it = CHARSET_CODES.begin();
         it != CHARSET_CODES.end();
         ++it) {

        keys.push_back(it->first);
    }

    return keys;
}

const int EncodingConverter::TINICONV_OPTION = 0;

// FIXME: 400 000 = memory access violation at address: 0xbfa729ac: no mapping at fault address
const int EncodingConverter::BUFFER_SIZE = 100000;

EncodingConverter::EncodingConverter() : defaultEncoding_("UTF-8") { }

EncodingConverter::EncodingConverter(std::string defaultEncoding)
    : defaultEncoding_(defaultEncoding) { }

std::string EncodingConverter::detect(std::string text) {
    std::string charset(ASCII_CHARSET);

    if (!detect_(text.c_str(), text.length(), charset)) {
        WARN("undetected encoding");
    }

    return charset;
}

bool EncodingConverter::detect_(const char* input, size_t length, std::string& output) {
    uchardet_t handle = uchardet_new();

    int result = uchardet_handle_data(handle, input, length);
    if (result != 0) {
        WARN("uchardet handling data error");
        return false;
    }
    uchardet_data_end(handle);

    output = uchardet_get_charset(handle);
    DEBUG("uchardet detected encoding: " << output);

    uchardet_delete(handle);

    return true;
}

std::string EncodingConverter::convert(std::string from, std::string to, std::string text) {
    if (!CHARSET_CODES.count(from)) {
        WARN("unrecognized source encoding: " << from);
        return text;
    }
    if (!CHARSET_CODES.count(to)) {
        WARN("unrecognized target encoding: " << to);
        return text;
    }

    std::string convertedText("");

    if (!convert_(CHARSET_CODES[from], CHARSET_CODES[to], text, convertedText)) {
        return text;
    }

    return convertedText;
}

bool EncodingConverter::convert_(int inCharsetId, int outCharsetId,
                                 std::string input, std::string& output) {

    tiniconv_ctx_s tiniconvStruct;
    int result = tiniconv_init(inCharsetId, outCharsetId, TINICONV_OPTION, &tiniconvStruct);

    if (result != TINICONV_INIT_OK) {
        WARN("tiniconv initialization failed");
        return false;
    }

    int inSizeConsumed, outSizeConsumed;

    unsigned char outputBuffer[BUFFER_SIZE];
    const char* inputChars = input.c_str();
    int inputCharsLength = strlen(inputChars);

    result = tiniconv_convert(&tiniconvStruct,
                              (const unsigned char*)inputChars, inputCharsLength, &inSizeConsumed,
                              outputBuffer, sizeof(outputBuffer) - 1, &outSizeConsumed);

    if (result < TINICONV_CONVERT_OUT_TOO_SMALL) {
        WARN("tiniconv convertion failed");
        return false;
    }

    outputBuffer[outSizeConsumed] = 0;
    output = std::string((const char *)outputBuffer);

    DEBUG("tiniconv convertion output: [" << output << "]");

    return true;
}

std::string EncodingConverter::convert(std::string to, std::string text) {
    return convert(detect(text), to, text);
}

std::string EncodingConverter::convert(std::string text) {
    return convert(detect(text), defaultEncoding_, text);
}
