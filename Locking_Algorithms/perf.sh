echo '----- PERF TESTS FOR "COUNTER" (10 threads & 1000 iterations) ------'
echo '------- L1 Cache Misses, Branch Predictions, and Page Faults -------'
echo ''
echo ''
echo '----------------------------- BARRIERS -----------------------------'
echo '___________________________ SENSE BARRIER __________________________'
perf stat --repeat 10 -e L1-dcache-loads,L1-dcache-load-misses,branch-loads,branch-load-misses,page-faults ./counter -t 10 -i=1000 --bar=sense -o out.txt
echo '__________________________ PTHREAD BARRIER _________________________'
perf stat --repeat 10 -e L1-dcache-loads,L1-dcache-load-misses,branch-loads,branch-load-misses,page-faults ./counter -t 10 -i=1000 --bar=pthread -o out.txt
echo ''
echo ''
echo ''
echo '------------------------------ LOCKS ------------------------------'
echo '_____________________________ TAS LOCK ____________________________'
perf stat --repeat 10 -e L1-dcache-loads,L1-dcache-load-misses,branch-loads,branch-load-misses,page-faults ./counter -t 10 -i=1000 --lock=tas -o out.txt
echo '_____________________________ TTAS LOCK ___________________________'
perf stat --repeat 10 -e L1-dcache-loads,L1-dcache-load-misses,branch-loads,branch-load-misses,page-faults ./counter -t 10 -i=1000 --lock=ttas -o out.txt
echo '____________________________ TICKET LOCK __________________________'
perf stat --repeat 10 -e L1-dcache-loads,L1-dcache-load-misses,branch-loads,branch-load-misses,page-faults ./counter -t 10 -i=1000 --lock=ticket-o out.txt
echo '____________________________ PTHREAD LOCK _________________________'
perf stat --repeat 10 -e L1-dcache-loads,L1-dcache-load-misses,branch-loads,branch-load-misses,page-faults ./counter -t 10 -i=1000 --lock=pthread -o out.txt
echo ''
echo ''
echo ''
echo ''
echo ''
echo ''
echo ''
echo ''
echo '----- PERF TESTS FOR "BUCKETSORT" (10 threads & 1000 integers) -----'
echo '------- L1 Cache Misses, Branch Predictions, and Page Faults -------'
echo ''
echo ''
echo '----------------------------- BARRIERS -----------------------------'
echo '___________________________ SENSE BARRIER __________________________'
perf stat --repeat 10 -e L1-dcache-loads,L1-dcache-load-misses,branch-loads,branch-load-misses,page-faults ./mysort source.txt -o out.txt -t 10 --alg=bucket --bar=sense
echo '__________________________ PTHREAD BARRIER _________________________'
perf stat --repeat 10 -e L1-dcache-loads,L1-dcache-load-misses,branch-loads,branch-load-misses,page-faults ./mysort source.txt -o out.txt -t 10 --alg=bucket --bar=pthread
echo ''
echo ''
echo ''
echo '------------------------------ LOCKS ------------------------------'
echo '_____________________________ TAS LOCK ____________________________'
perf stat --repeat 10 -e L1-dcache-loads,L1-dcache-load-misses,branch-loads,branch-load-misses,page-faults ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=tas
echo '_____________________________ TTAS LOCK ___________________________'
perf stat --repeat 10 -e L1-dcache-loads,L1-dcache-load-misses,branch-loads,branch-load-misses,page-faults ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=ttas
echo '____________________________ TICKET LOCK __________________________'
perf stat --repeat 10 -e L1-dcache-loads,L1-dcache-load-misses,branch-loads,branch-load-misses,page-faults ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=ticket
echo '____________________________ PTHREAD LOCK _________________________'
perf stat --repeat 10 -e L1-dcache-loads,L1-dcache-load-misses,branch-loads,branch-load-misses,page-faults ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=pthread
