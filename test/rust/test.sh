set -e
../../build_Debug/bin/bft ../hworld.bf -o hworld.rs --target=rust
../../build_Debug/bin/bft ../mandelbrot.bf -o mandelbrot.rs --target=rust
rustc ./hworld.rs -o hworld --extern libc=/home/xavier/rust_bf/target/debug/deps/liblibc-2631bfb539974b41.rlib
rustc ./mandelbrot.rs -o mandelbrot --extern libc=/home/xavier/rust_bf/target/debug/deps/liblibc-2631bfb539974b41.rlib
./hworld
echo "
"
./mandelbrot
echo "
"