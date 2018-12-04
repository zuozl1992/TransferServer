#!/bin/bash

find ./* -exec touch {} \;
qmake
make
