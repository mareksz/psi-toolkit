#include "gobio_limit_checker.hpp"


GobioLimitChecker::GobioLimitChecker(std::string lang) {
    if (lang == "pl") {
        setAbsoluteLimit(10);
        addUnlimitedSpec(30);
        addLimitSpec(40, 200);
        addLimitSpec(50, 140);
        addLimitSpec(70, 80);
        addLimitSpec(100, 15);
    }
}
