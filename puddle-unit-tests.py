#!/usr/bin/python

from __future__ import print_function
import PSIToolkit, sys

fail_count = 0
total_count = 0
with open('/psi-toolkit/tools/parsers/puddle/data/pl/rules.pl') as rules_file:
    for line in rules_file:
        if line.startswith('#>'):
            fields = line.rstrip().split('#>')
            test_phrase = fields[1]
            condition = fields[2]
            print("Checking phrase: '%s' of length %d, condition: %s ..." % (test_phrase, len(test_phrase), condition), end='')
            total_count += 1
            psi = PSIToolkit.PipeRunner('tokenize --lang pl ! lemmatize ! puddle')
            result = psi.run(test_phrase)
            test_ok = False
            for line in result.split('\n'):
                edge = line.split()
                try:
                    begin = int(edge[1])
                    end = int(edge[2])
                    tags = edge[4]
                    if tags == '!pl,parse,puddle':
                        annotation = edge[5]
                    elif tags == '!pl,puddle,token':
                        annotation = edge[6]
                    else:
                        continue
                    #print("Checking edge: begin=%d end=%d annotation=%s" % (begin, end, annotation))
                    if begin == 0 and end == len(test_phrase) and annotation.startswith(condition):
                        test_ok = True
                        break
                except:
                    pass
            if test_ok:
                print('OK')
            else:
                print('FAIL')
                fail_count += 1
                #print(result)
print("Tests count = %d, fail count = %d" % (total_count, fail_count))
