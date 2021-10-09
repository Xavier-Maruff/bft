set -e
../../build_Debug/bin/bft ../hworld.bf -o hworld.c --target=c
../../build_Debug/bin/bft ../mandelbrot.bf -o mandelbrot.c --target=c
gcc ./hworld.c -o hworld
gcc ./mandelbrot.c -o mandelbrot
./hworld
echo "
"
./mandelbrot
echo "
"
rm -rf ./hworld* ./mandelbrot*