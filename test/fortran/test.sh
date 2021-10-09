set -e
../../build_Debug/bin/bft ../hworld.bf -o hworld.f90 --target=fortran
../../build_Debug/bin/bft ../mandelbrot.bf -o mandelbrot.f90 --target=fortran
gfortran mandelbrot.f90 -o mandelbrot
gfortran hworld.f90 -o hworld
./hworld
echo "
"
./mandelbrot
echo "
"
rm -rf ./hworld* ./mandelbrot*