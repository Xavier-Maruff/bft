set -e
../../build_Debug/bin/bft ../hworld.bf -o hworld.js --target=js
../../build_Debug/bin/bft ../mandelbrot.bf -o mandelbrot.js --target=js
node hworld.js
echo "
"
node mandelbrot.js
echo "
"