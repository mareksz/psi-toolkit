#!/bin/bash

if [ $# -eq 0 ] \
    || [ $1 == "--address" ] \
    || [ $1 == "--port" ] \
    || [ $1 == "--threads" ] \
    || [ $1 == "--root" ] \
    || [ $1 == "--daemon" ] \
    || [ $1 == "--leave-standard-descriptors-when-daemonizing" ] \
    || [ $1 == "--verbose" ]
then
    ./server/psi-server "$@"
else
    ./framework/psi-pipe "$@"
fi
