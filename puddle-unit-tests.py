#!/usr/bin/python

from __future__ import print_function
import PSIToolkit, sys, time

fail_count = 0
with open('/psi-toolkit/tools/parsers/puddle/data/pl/rules.pl') as rules_file:
    for line in rules_file:
        print("Analyzing line: "+line.rstrip())
        if line.startswith('#>'):
            fields = line.rstrip().split('#>')
            print(fields)
            test_phrase = fields[1]
            condition = fields[2]
            print("Checking phrase: '%s', condition: %s ..." % (test_phrase, condition), end='')
            psi = PSIToolkit.PipeRunner('tokenize --lang pl ! lemmatize ! puddle')
            result = psi.run(test_phrase)

            print("OK")
            print(result)
            #time.sleep(1)
