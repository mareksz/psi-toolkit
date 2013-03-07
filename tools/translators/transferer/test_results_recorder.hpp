#pragma once

/*
  Copyright (C) 2007-2011 Poleng Sp. z o.o.

  This file can be redistributed and/or modified under the terms of the
  GNU Lesser General Public Licence as published by the Free Software
  Foundation.
*/

#include <string>

class TestResultsRecorder
{
public:
    TestResultsRecorder();
    unsigned int getNumberOfTests() const;
    unsigned int getNumberOfSuccesses() const;
    unsigned int getNumberOfFailures() const;
    unsigned int getNumberOfSkipped() const;

    bool wereTestsPassed() const;

    void markAsSuccess(const std::string& msg = "");
    void markAsFailure(const std::string& msg = "");
    void markAsSkipped(const std::string& msg = "");

private:
    void print_msg_(const std::string& head, const std::string& msg);

    unsigned int number_of_successes;
    unsigned int number_of_failures;
    unsigned int number_of_skipped;
};
