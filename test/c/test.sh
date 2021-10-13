set -e
../../build_Debug/bin/bft ../hworld.bf -o hworld.c --target=c
../../build_Debug/bin/bft ../mandelbrot.bf -o mandelbrot.c --target=c
gcc -g ./hworld.c -o hworld
gcc -g ./mandelbrot.c -o mandelbrot
./hworld
echo "
"
./mandelbrot
echo "
"
rm -rf ./hworld* ./mandelbrot*
