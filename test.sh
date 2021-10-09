set -e
echo "Building..."
./build.sh Debug
echo "Running C target"
cd test/c
./test.sh
echo "
Running JS target"
cd ../js
./test.sh
echo "
Running Rust target"
cd ../rust
./test.sh
echo "
Running Ada target"
cd ../ada
./test.sh
echo "
Running Fortran target"
cd ../fortran
./test.sh
echo "
Testing finished"
