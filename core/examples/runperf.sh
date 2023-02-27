#!/bin/sh
perf record -F 9999 -g -- ./build/yijinjing-demo
perf report -n --stdio
