/*
  Copyright (C) 2007-2011 Poleng Sp. z o.o.

  This file can be redistributed and/or modified under the terms of the
  GNU Lesser General Public Licence as published by the Free Software
  Foundation.
*/

#include "test_results_recorder.hpp"

#include <iostream>

TestResultsRecorder::TestResultsRecorder():
    number_of_successes(0),
    number_of_failures(0),
    number_of_skipped(0)
{
}

unsigned int TestResultsRecorder::getNumberOfTests() const
{
    return number_of_successes + number_of_failures + number_of_skipped;
}

unsigned int TestResultsRecorder::getNumberOfSuccesses() const
{
    return number_of_successes;
}

unsigned int TestResultsRecorder::getNumberOfFailures() const
{
    return number_of_failures;
}

unsigned int TestResultsRecorder::getNumberOfSkipped() const
{
    return number_of_skipped;
}

bool TestResultsRecorder::wereTestsPassed() const
{
    return number_of_failures == 0;
}

void TestResultsRecorder::markAsSuccess(const std::string& msg)
{
    ++number_of_successes;
    print_msg_("ok", msg);
}

void TestResultsRecorder::markAsFailure(const std::string& msg)
{
    ++number_of_failures;
    print_msg_("not ok", msg);
}

void TestResultsRecorder::markAsSkipped(const std::string& msg)
{
    ++number_of_skipped;
    print_msg_("skipped", msg);
}

void TestResultsRecorder::print_msg_(const std::string& head, const std::string& msg)
{
    unsigned int current_test_number = getNumberOfTests();

    std::string formatted_msg =
        (msg.empty() ? std::string("") : (std::string(" ") + msg));

    std::cerr << head << ' ' << current_test_number << formatted_msg << std::endl;
}
