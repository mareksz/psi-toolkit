#!/usr/bin/python

from __future__ import print_function
import PSIToolkit, sys


if len(sys.argv) > 2:
    start_line = int(sys.argv[2])
else:
    start_line = 1

psi = PSIToolkit.PipeRunner('tokenize --lang pl ! lemmatize ! puddle')
line_index = 0


with open(sys.argv[1]) as in_file:
    for line in in_file:
        line_index += 1
        if line_index >= start_line:
            print(line.rstrip())
            result = psi.run(line.rstrip())
            printed_edges = []
            for edge_line in result.split('\n'):
                edge = edge_line.split()
                try:
                    begin = int(edge[1])
                    length = int(edge[2])
                    tags = edge[4]
                    annotations = edge[5]
                    if tags == '!pl,parse,puddle' and annotations.startswith('NP') and not (begin,length) in printed_edges:
                        print("NP: "+line[begin:begin+length])
                        printed_edges.append((begin, length))


                except:
                    pass
            if line_index % 10 == 0:
                sys.stderr.write("Done line %d\n" % line_index)
