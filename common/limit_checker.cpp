#include "limit_checker.hpp"


#include <limits>

#include <boost/foreach.hpp>

#include "logging.hpp"


LimitChecker::LimitChecker() :
    ready_(true),
    absoluteLimit_((std::numeric_limits<int>::max)())
{ }


LimitChecker::LimitChecker(int absoluteLimit) :
    ready_(true),
    absoluteLimit_(absoluteLimit)
{ }


void LimitChecker::setAbsoluteLimit(int absoluteLimit) {
    absoluteLimit_ = absoluteLimit;
}


void LimitChecker::addLimitSpec(int argument, int limit) {
    if (!specs_.empty() && argument < specs_.back().argument) {
        ready_ = false;
    }
    LimitChecker::LimitSpec spec(argument, limit);
    specs_.push_back(spec);
}


void LimitChecker::addUnlimitedSpec(int argument) {
    addLimitSpec(argument, (std::numeric_limits<int>::max)());
}


bool LimitChecker::isAllowed(int argument, int value) {
    if (!ready_) {
        specs_.sort();
        ready_ = true;
    }
    BOOST_FOREACH(LimitSpec spec, specs_) {
        if (spec.argument >= argument) {
if (value > spec.limit) DEBUG("Limit " << spec.limit << " exceeded at edge length " << argument);
            return value <= spec.limit;
        }
    }
if (value > absoluteLimit_) DEBUG("Absolute limit " << absoluteLimit_ << " exceeded.");
    return value <= absoluteLimit_;
}
