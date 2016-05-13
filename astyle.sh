#!/bin/sh

astyle --options=.astylerc --suffix=none --recursive --exclude=src/thirdparty "src/*.cpp" "src/*.h"
