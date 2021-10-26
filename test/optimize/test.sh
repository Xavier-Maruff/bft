../../build_Debug/bin/bft benchmark.bf -o benchmark_0.c --optimize=0 -t c -v
echo "
"
../../build_Debug/bin/bft benchmark.bf -o benchmark_1.c --optimize=1 -t c -v
echo "
"
../../build_Debug/bin/bft benchmark.bf -o benchmark_2.c --optimize=2 -t c -v
echo "
"
../../build_Debug/bin/bft benchmark.bf -o benchmark_3.c --optimize=3 -t c -v

echo "

"
../../build_Debug/bin/bft benchmark.bf -o bf_1.bf --optimize=1 -d -v
echo "
"
../../build_Debug/bin/bft benchmark.bf -o bf_2.bf --optimize=2 -d -v
echo "
"

gcc -g ./benchmark_0.c -o benchmark_0
gcc -g ./benchmark_1.c -o benchmark_1
gcc -g ./benchmark_2.c -o benchmark_2
gcc -g ./benchmark_3.c -o benchmark_3

echo "Optimization level: 0"
time ./benchmark_0

echo "

Optimization level: 1"
time ./benchmark_1

echo "

Optimization level: 2"
time ./benchmark_2

echo "

Optimization level: 3"
time ./benchmark_3
rm -rf benchmark_*

