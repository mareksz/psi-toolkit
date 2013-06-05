#ifndef LIMIT_CHECKER_HDR
#define LIMIT_CHECKER_HDR


#include <list>


/**
 * Class LimitChecker allows for checking if a given value is within some predefined limit.
 *
 * Limit can be absolute or dependent on the given argument.
 * E.g. absolute limit 8 means that values <=8 are allowed and values >8 are forbidden,
 * regardless of the given argument.
 *
 * Example of the complex limit specification:
 *
 *   LimitChecker lc(8);
 *   lc.addUnlimitedSpec( 10);
 *   lc.addLimitSpec    ( 20, 30);
 *   lc.addLimitSpec    ( 30, 20);
 *   lc.addLimitSpec    (100, 12);
 *
 * It gives the following limits:
 *
 *   For arguments           <= 10  there is no limit (the limit is infinity).
 *   For arguments  > 10 and <= 20  the limit is  30.
 *   For arguments  > 20 and <= 30  the limit is  20.
 *   For arguments  > 30 and <=100  the limit is  12.
 *   For arguments  >100            the limit is   8.
 *
 *
 */
class LimitChecker {
public:

    /**
     * Create a limit checker with no limits.
     */
    LimitChecker();

    /**
     * Create a limit checker with the given absolute limit.
     */
    LimitChecker(int absoluteLimit);

    /**
     * Set absolute limit.
     */
    void setAbsoluteLimit(int absoluteLimit);

    /**
     * Add limit specification for the given argument.
     */
    void addLimitSpec(int argument, int limit);

    /**
     * Add "infinite" limit for the given argument.
     */
    void addUnlimitedSpec(int argument);

    /**
     * Check if the given value is within the limits for the given argument.
     */
    bool isAllowed(int argument, int value);

private:
    struct LimitSpec {
        int argument;
        int limit;

        LimitSpec(int an_argument, int a_limit) : argument(an_argument), limit(a_limit) { }

        bool operator<(const LimitSpec & other) const {
            return argument < other.argument;
        }
    };

    bool ready_;
    int absoluteLimit_;
    std::list<LimitSpec> specs_;
};


#endif
