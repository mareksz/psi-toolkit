#include "limit_checker.hpp"


#include <limits>

#include <boost/foreach.hpp>


LimitChecker::LimitChecker() :
    ready_(true),
    absoluteLimit_(std::numeric_limits<int>::max())
{ }


LimitChecker::LimitChecker(int absoluteLimit) :
    ready_(true),
    absoluteLimit_(absoluteLimit)
{ }


void LimitChecker::setAbsoluteLimit(int absoluteLimit) {
    absoluteLimit_ = absoluteLimit;
}


void LimitChecker::addLimitSpec(int argument, int limit) {
    if (argument < specs_.back().argument) {
        ready_ = false;
    }
    specs_.push_back(LimitChecker::LimitSpec(argument, limit));
}


void LimitChecker::addUnlimitedSpec(int argument) {
    addLimitSpec(argument, std::numeric_limits<int>::max());
}


bool LimitChecker::isAllowed(int argument, int value) {
    if (!ready_) {
        specs_.sort();
        ready_ = true;
    }
    BOOST_FOREACH(LimitSpec spec, specs_) {
        if (spec.argument >= argument) {
            return value <= spec.limit;
        }
    }
    return value <= absoluteLimit_;
}
