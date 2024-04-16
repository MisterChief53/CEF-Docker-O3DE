#!/bin/bash

xvfb-run --server-args="-screen 0 1024x768x24" $HOME/${projectDirName}/${workspaceHash}/out/build/cef-apps-Linux-GCC-Debug/tests/cefsimple/Debug/cefsimple "$@"

# $HOME/${projectDirName}/${workspaceHash}/src/tests/cefsimple/run-cefsimple.sh