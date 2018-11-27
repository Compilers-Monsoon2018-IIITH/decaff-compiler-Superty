#!/bin/bash

./adc < $1 > a.ll
clang a.ll
rm a.ll
