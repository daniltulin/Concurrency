mkdir -p sources
cd sources
svn co http://llvm.org/svn/llvm-project/llvm/trunk llvm > /dev/null
cd llvm/tools
svn co http://llvm.org/svn/llvm-project/cfe/trunk clang > /dev/null
cd ../..
cd llvm/projects
svn co http://llvm.org/svn/llvm-project/compiler-rt/trunk compiler-rt > /dev/null
cd ../..
mkdir build 
cd build
cmake -G "Unix Makefiles" ../llvm
