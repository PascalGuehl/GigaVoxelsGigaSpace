#!/bin/sh
# Exit script on error
set -e

cd "$(dirname "$0")"

cd ../../Development/Documents/Doxygen
doxygen DoxyfileGigaSpace.cfg
