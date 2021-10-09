set -e
../../build_Debug/bin/bft ../hworld.bf -o hworld.adb --target=ada
../../build_Debug/bin/bft ../mandelbrot.bf -o mandelbrot.adb --target=ada
gnatmake hworld.adb -o hworld
gnatmake mandelbrot.adb -o mandelbrot
./hworld
echo "
"
./mandelbrot
echo "
"
rm -rf ./hworld* ./mandelbrot*
