set -e
echo "Building..."
./build.sh Debug
echo "Running C target"
cd test/c
../../build_Debug/bin/bft ../hworld.bf -o hworld.c --target=c
gcc ./hworld.c -o hworld
./hworld
echo "
Running JS target"
cd ../js
../../build_Debug/bin/bft ../hworld.bf -o hworld.js --target=js
node hworld.js
cd ../..
echo "
Testing finished"
