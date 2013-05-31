#include "gobio_limit_checker.hpp"


GobioLimitChecker::GobioLimitChecker(std::string lang) {
    if (lang == "pl") {
        setAbsoluteLimit(8);
        addUnlimitedSpec(10);
        addLimitSpec(20, 30);
        addLimitSpec(30, 20);
        addLimitSpec(100, 12);
    }
}
