all:
	g++ test.cc -std=c++14 -o ThreadPoolTest 

test: all
	./ThreadPoolTest; rm ThreadPoolTest

debug: 
	g++ test.cc -std=c++14 -g -o ThreadPoolTest; sudo gdb ThreadPoolTest; rm -r ThreadPoolTest *.dSYM

clean:
	rm ThreadPoolTest
