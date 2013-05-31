#ifndef GOBIO_LIMIT_CHECKER_HDR
#define GOBIO_LIMIT_CHECKER_HDR


#include <string>

#include "limit_checker.hpp"


/**
 * Limit checker for Gobio parser.
 * It has some predefined values for some languages.
 */
class GobioLimitChecker : protected LimitChecker {
public:

    /**
     * Create a limit checker for the given language.
     */
    GobioLimitChecker(std::string lang);

    /**
     * Check if the given value is within the limits for the given argument.
     * Call similar superclass's method.
     */
    bool isAllowed(int argument, int value);

};


#endif
