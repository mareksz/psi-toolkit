#include "gobio_limit_checker.hpp"


GobioLimitChecker::GobioLimitChecker(std::string lang) {
    if (lang == "pl") {
        setAbsoluteLimit(8);
        addUnlimitedSpec(20);
        addLimitSpec(30, 50);
        addLimitSpec(40, 20);
        addLimitSpec(100, 12);
    }
}
