#!/bin/bash
# http://www.hydrogen18.com/blog/howto-give-a-single-process-its-own-cpu-core-in-linux.html
# Script to launch a process at maximum priority
# on a single specified cpu
# usage:
# ./reserve_cpu 2 my_command
set -e

cpu_count=$(grep processor /proc/cpuinfo | wc -l)
processor=$1
process_mask=$((1 << ${processor?}))
all_cpus=$((2 ** ${cpu_count?} -1))
irq_mask=$((${all_cpus?} ^ ${process_mask?}))

# This must be converted to hex
irq_mask=$(echo "obase=16; ${irq_mask?}" | bc)
set +e
find /proc/irq/ -type f -name smp_affinity | sudo xargs -L 1 -I '{}' /bin/bash -c "echo ${irq_mask?} > {} 2>/dev/null"
set -e

exec nice -n -19 taskset "${process_mask?}" ${@:2:99}
