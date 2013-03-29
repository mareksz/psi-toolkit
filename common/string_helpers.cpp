#include "string_helpers.hpp"

#include "utf8/utf8.h"
#include "config.hpp"

size_t symbolLength(const std::string& s, int ix) {
    return symbolLength(s.c_str(), ix);
}

size_t symbolLength(const StringFrag& s, int ix) {
    return symbolLength(s.str(), ix);
}

size_t symbolLength(const char* s, int ix) {
    return symbolLength(s + ix);
}

size_t symbolLength(const char* s) {
    size_t len = utf8::unchecked::sequence_length(s);

    return len > 0 ? len : 1;
}

std::string getLemmaFromLexeme(const std::string& lexeme) {
    size_t separatorPos = lexeme.find_first_of(LEMMA_CATEGORY_SEPARATOR);

    if (separatorPos == std::string::npos)
        return lexeme;

    return lexeme.substr(0, separatorPos);
}
