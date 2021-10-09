set -e
../../build_Debug/bin/bft ../hworld.bf -o hworld.js --target=js
#../../build_Debug/bin/bft ../mandelbrot.bf -o mandelbrot.js --target=js
node hworld.js
echo "
"

#NOTE: JS Mandelbrot test takes a LONG time, better to leave it out until optimization better
#node mandelbrot.js
#echo "
#"

rm -rf ./hworld* ./mandelbrot*