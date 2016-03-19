mkdir -p sources
cd sources
svn co http://llvm.org/svn/llvm-project/llvm/trunk llvm > /dev/null && echo 'llvm is fetched'
cd llvm/tools
svn co http://llvm.org/svn/llvm-project/cfe/trunk clang > /dev/null && echo 'clang is fetched'
cd ../..
cd llvm/projects
svn co http://llvm.org/svn/llvm-project/compiler-rt/trunk compiler-rt > /dev/null && echo 'compiler-rt is fetched'
cd ../..
mkdir build 
cd build
cmake -G "Unix Makefiles" ../llvm
make install 
ls
CXX -v
