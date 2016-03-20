mkdir -p build
cd build
cmake .. && make && make test ARGS=-V
