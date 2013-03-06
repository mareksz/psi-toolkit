#include "tests.hpp"

#include "numconversion.hpp"

BOOST_AUTO_TEST_SUITE( transferer )

BOOST_AUTO_TEST_CASE( numconversion ) {

    std::string tequiv;
    std::string tinflection;
    std::string tsyntax;

    ncnv::num2pltext(
        ncnv::ORD,
        "53",
        tequiv,
        tinflection,
        tsyntax);

    BOOST_CHECK_EQUAL(tequiv, "pięćdziesiąty trzeci");
    BOOST_CHECK_EQUAL(tinflection, "LP-P LP-P");
    BOOST_CHECK_EQUAL(tsyntax, "");
}

BOOST_AUTO_TEST_SUITE_END()
