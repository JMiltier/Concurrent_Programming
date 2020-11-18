#!/bin/bash
cd bin/
echo "***********NUMBER OF THREADS: 10, NUM OF ITERATIONS 50000**********"
echo "XXXXXXXXXXXXX----------------MICROBENCHMARK----------------XXXXXXXXXXX"
echo "   "
echo "@@@@@@@@@@......Perf Analysis of Microbenchmark barriers......@@@@@@@@@@"
echo "###########------Cache misses------############"
perf stat -e cache-misses ./counter -t 10 -i=100 --bar=pthread -o out.txt
perf stat -e cache-misses ./counter -t 10 -i=100 --bar=sense -o out.txt
echo "###########------L1-dcache-loads------############"
perf stat -e L1-dcache-loads ./counter -t 10 -i=100 --bar=pthread -o out.txt
perf stat -e L1-dcache-loads ./counter -t 10 -i=100 --bar=sense -o out.txt
echo "###########------L1-dcache-load-misses------############"
perf stat -e L1-dcache-load-misses ./counter -t 10 -i=100 --bar=pthread -o out.txt
perf stat -e L1-dcache-load-misses ./counter -t 10 -i=100 --bar=sense -o out.txt
echo "###########------branch-load-misses------############"
perf stat -e branch-load-misses ./counter -t 10 -i=100 --bar=pthread -o out.txt
perf stat -e branch-load-misses ./counter -t 10 -i=100 --bar=sense -o out.txt
echo "###########------branch-loads------############"
perf stat -e branch-loads ./counter -t 10 -i=100 --bar=pthread -o out.txt
perf stat -e branch-loads ./counter -t 10 -i=100 --bar=sense -o out.txt
echo "###########------page faults------############"
perf stat -e page-faults ./counter -t 10 -i=100 --bar=pthread -o out.txt
perf stat -e page-faults ./counter -t 10 -i=100 --bar=sense -o out.txt
echo "  "
echo "@@@@@@@@@@......Perf Analysis of Microbenchmark Locks......@@@@@@@@@@"
echo "###########------Cache misses------############"
perf stat -e cache-misses ./counter -t 10 -i=100 --lock=tas -o out.txt
perf stat -e cache-misses ./counter -t 10 -i=100 --lock=ttas -o out.txt
perf stat -e cache-misses ./counter -t 10 -i=100 --lock=ticket -o out.txt
perf stat -e cache-misses ./counter -t 10 -i=100 --lock=mcs -o out.txt
perf stat -e cache-misses ./counter -t 10 -i=100 --lock=pthread -o out.txt
echo "###########------L1-dcache-loads------############"
perf stat -e L1-dcache-loads ./counter -t 10 -i=100 --lock=tas -o out.txt
perf stat -e L1-dcache-loads ./counter -t 10 -i=100 --lock=ttas -o out.txt
perf stat -e L1-dcache-loads ./counter -t 10 -i=100 --lock=ticket -o out.txt
perf stat -e L1-dcache-loads ./counter -t 10 -i=100 --lock=mcs -o out.txt
perf stat -e L1-dcache-loads ./counter -t 10 -i=100 --lock=pthread -o out.txt
echo "###########------L1-dcache-load-misses------############"
perf stat -e L1-dcache-load-misses ./counter -t 10 -i=100 --lock=tas -o out.txt
perf stat -e L1-dcache-load-misses ./counter -t 10 -i=100 --lock=ttas -o out.txt
perf stat -e L1-dcache-load-misses ./counter -t 10 -i=100 --lock=ticket -o out.txt
perf stat -e L1-dcache-load-misses ./counter -t 10 -i=100 --lock=mcs -o out.txt
perf stat -e L1-dcache-load-misses ./counter -t 10 -i=100 --lock=pthread -o out.txt
echo "###########------branch-load-misses------############"
perf stat -e branch-load-misses ./counter -t 10 -i=100 --lock=tas -o out.txt
perf stat -e branch-load-misses ./counter -t 10 -i=100 --lock=ttas -o out.txt
perf stat -e branch-load-misses ./counter -t 10 -i=100 --lock=ticket -o out.txt
perf stat -e branch-load-misses ./counter -t 10 -i=100 --lock=mcs -o out.txt
perf stat -e branch-load-misses ./counter -t 10 -i=100 --lock=pthread -o out.txt
echo "###########------branch-loads------############"
perf stat -e branch-loads ./counter -t 10 -i=100 --lock=tas -o out.txt
perf stat -e branch-loads ./counter -t 10 -i=100 --lock=ttas -o out.txt
perf stat -e branch-loads ./counter -t 10 -i=100 --lock=ticket -o out.txt
perf stat -e branch-loads ./counter -t 10 -i=100 --lock=mcs -o out.txt
perf stat -e branch-loads ./counter -t 10 -i=100 --lock=pthread -o out.txt
echo "###########------page faults------############"
perf stat -e page-faults ./counter -t 10 -i=100 --lock=tas -o out.txt
perf stat -e page-faults ./counter -t 10 -i=100 --lock=ttas -o out.txt
perf stat -e page-faults ./counter -t 10 -i=100 --lock=ticket -o out.txt
perf stat -e page-faults ./counter -t 10 -i=100 --lock=mcs -o out.txt
perf stat -e page-faults ./counter -t 10 -i=100 --lock=pthread -o out.txt