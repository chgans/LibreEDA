#!/bin/sh

cd src
cppcheck --quiet \
    --enable=warning,style,performance,portability \
    --std=c++11 \
    --inconclusive \
    -I thirdparty/ -I libs/ -I plugins/ \
    app \
    libs \
    plugins \
    2>&1
