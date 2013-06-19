#!/bin/bash

# Arch packages needed for running this: python-distribute, python-nose

TARGET_DIR=build
SKIP_STYLE_CHECKING_IN_EXTERNAL_LIBS=true
FILE_EXTS="cpp|hpp|h|c"

# Automatically generated files are excluded from the check
CPPCHECK_EXCLUDE="-i ${TARGET_DIR}/bindings/perl -i ${TARGET_DIR}/bindings/python -i ${TARGET_DIR}/bindings/ruby -i tools/parsers/gobio/translator/lex.grlex.cpp"

# Regexp for all external libraries incorporated into PSI-Toolkit
EXTERNAL_LIBS="server/mpfd-parser|utf8|maxent|sundown|fex|tiniconv|uchardet|tools/translators/bonsai/irstlm"

# Regexp for automatically generated files by bison etc.
AUTO_GENERATED_FILES=".*tools/parsers/gobio/translator/lex.(g|t)rlex.cpp|.*tools/parsers/gobio/translator/grparser.bis.cpp"

mkdir -p $TARGET_DIR

cd $TARGET_DIR
make -j 6
../main-tests.sh

cd ..

# Warning: requires following perl packages:
# TAP::Formatter::JUnit
# TAP::Harness
./tests/perl-test-runner.sh
./tests/python-test-runner.sh
./tests/ruby-test-runner.sh

cd $TARGET_DIR

PATH="./framework:${PATH}" PERL5LIB="../../cltu:${PERL5LIB}" perl ../../cltu/cltu.pl ../framework/t/general.ush --log cltu.log

# Running valgrind
valgrind --xml=yes --xml-file=valgrind.xml --suppressions=../valgrind.supp ./tests/test_runner
xsltproc  ~/valgrind-reports-to-xunit/xslt/valgrind_transform.xsl valgrind.xml > valgrindTestResults.xml

cd ..

# Running cppcheck
cppcheck -D__cplusplus -D__GNUC__=3 -f --xml . --enable=all echo `find . -type d ! -path './.git*' ! -path "./${TARGET_DIR}"'*' | perl -ne 'chomp; print "-I$_ "'` ${CPPCHECK_EXCLUDE} 2> cppcheck-result.xml.all

# Running Vera++
VERA_EXCLUDE="(${TARGET_DIR}|${EXTERNAL_LIBS}|${AUTO_GENERATED_FILES})"

find . -regextype posix-extended -regex ".*\.(${FILE_EXTS})" ! -regex "\./${VERA_EXCLUDE}/.*" ! -path './common/config.hpp' | vera++ - -profile psi -showrules 2>&1 | ./vera++2cppcheck.pl > vera++-result.xml.all

if $SKIP_STYLE_CHECKING_IN_EXTERNAL_LIBS; then
    egrep -v "file=\"(${EXTERNAL_LIBS}|${AUTO_GENERATED_FILES}).*\" severity=\"style\"" cppcheck-result.xml.all > cppcheck-result.xml
    egrep -v "file=\"(${EXTERNAL_LIBS}|${AUTO_GENERATED_FILES}).*\" severity=\"style\"" vera++-result.xml.all > vera++-result.xml
else
    cp cppcheck-result.xml.all cppcheck-result.xml
    cp vera++-result.xml.all vera++-result.xml
fi

rm cppcheck-result.xml.all cppcheck-result.xml.all

# Running server tests
if ! type -P jruby > /dev/null; then
    echo -e "WARNING: jRuby has been not found!\n\n"
else
    cd server/t
    jruby runner.rb
    cd ../../
fi
