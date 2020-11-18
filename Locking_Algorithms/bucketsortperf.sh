echo "XXXXXXXXXXXXX----------------BUCKETSORT----------------XXXXXXXXXXX"
echo "   "
cho "@@@@@@@@@@......Perf Analysis of barriers,no of threads-> 20......@@@@@@@@@@"
echo "###########------Cache misses------############"
perf stat -e cache-misses ./mysort source.txt -o out.txt -t 10 --alg=bucket --bar=pthread
perf stat -e cache-misses ./mysort source.txt -o out.txt -t 10 --alg=bucket --bar=sense
echo "###########------L1-dcache-loads------############"
perf stat -e L1-dcache-loads ./mysort source.txt -o out.txt -t 10 --alg=bucket --bar=pthread
perf stat -e L1-dcache-loads ./mysort source.txt -o out.txt -t 10 --alg=bucket --bar=sense
echo "###########------L1-dcache-load-misses------############"
perf stat -e L1-dcache-load-misses ./mysort source.txt -o out.txt -t 10 --alg=bucket --bar=pthread
perf stat -e L1-dcache-load-misses ./mysort source.txt -o out.txt -t 10 --alg=bucket --bar=sense
echo "###########------branch-load-misses------############"
perf stat -e branch-load-misses ./mysort source.txt -o out.txt -t 10 --alg=bucket --bar=pthread
perf stat -e branch-load-misses ./mysort source.txt -o out.txt -t 10 --alg=bucket --bar=sense
echo "###########------branch-loads------############"
perf stat -e branch-loads ./mysort source.txt -o out.txt -t 10 --alg=bucket --bar=pthread
perf stat -e branch-loads ./mysort source.txt -o out.txt -t 10 --alg=bucket --bar=sense
echo "###########------page faults------############"
perf stat -e page-faults ./mysort source.txt -o out.txt -t 10 --alg=bucket --bar=pthread
perf stat -e page-faults ./mysort source.txt -o out.txt -t 10 --alg=bucket --bar=sense
echo "   "
echo "@@@@@@@@@@......Perf Analysis of bucket sort Locks, no of threads-> 20......@@@@@@@@@@"
echo "###########------Cache misses------############"
perf stat -e cache-misses ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=tas
perf stat -e cache-misses ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=ttas
perf stat -e cache-misses ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=ticket
perf stat -e cache-misses ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=pthread
echo "###########------L1-dcache-loads------############"
perf stat -e L1-dcache-loads ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=tas
perf stat -e L1-dcache-loads ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=ttas
perf stat -e L1-dcache-loads ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=ticket
perf stat -e L1-dcache-loads ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=pthread
echo "###########------L1-dcache-load-misses------############"
perf stat -e L1-dcache-load-misses ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=tas
perf stat -e L1-dcache-load-misses ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=ttas
perf stat -e L1-dcache-load-misses ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=ticket
perf stat -e L1-dcache-load-misses ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=pthread
echo "###########------branch-load-misses------############"
perf stat -e branch-load-misses ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=tas
perf stat -e branch-load-misses ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=ttas
perf stat -e branch-load-misses ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=ticket
perf stat -e branch-load-misses ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=pthread
echo "###########------branch-loads------############"
perf stat -e branch-loads ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=tas
perf stat -e branch-loads ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=ttas
perf stat -e branch-loads ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=ticket
perf stat -e branch-loads ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=pthread
echo "###########------page faults------############"
perf stat -e page-faults ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=tas
perf stat -e page-faults ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=ttas
perf stat -e page-faults ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=ticket
perf stat -e page-faults ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=pthread