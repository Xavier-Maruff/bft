rm -rf benchmark_out.txt
touch benchmark_out.txt

../../build_Debug/bin/bft benchmark.bf -o benchmark_0.c --optimize=0 -t c -v
../../build_Debug/bin/bft benchmark.bf -o benchmark_1.c --optimize=1 -t c -v
../../build_Debug/bin/bft benchmark.bf -o benchmark_2.c --optimize=2 -t c -v
../../build_Debug/bin/bft benchmark.bf -o benchmark_3.c --optimize=3 -t c -v

gcc ./benchmark_0.c -o benchmark_0
gcc ./benchmark_1.c -o benchmark_1
gcc ./benchmark_2.c -o benchmark_2
gcc ./benchmark_3.c -o benchmark_3

echo "Optimization level: 0"
time ./benchmark_0 > benchmark_out.txt

rm -rf benchmark_out.txt
touch benchmark_out.txt
echo "

Optimization level: 1"
time ./benchmark_1 > benchmark_out.txt

rm -rf benchmark_out.txt
touch benchmark_out.txt
echo "

Optimization level: 2"
time ./benchmark_2 > benchmark_out.txt

rm -rf benchmark_out.txt
touch benchmark_out.txt
echo "

Optimization level: 3"
time ./benchmark_3 > benchmark_out.txt
rm -rf benchmark_*
