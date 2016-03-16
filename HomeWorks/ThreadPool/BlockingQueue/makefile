all:
	g++ test.cc -std=c++14 -o BlockingQueueTest 

test: all
	./BlockingQueueTest; rm BlockingQueueTest

debug: 
	g++ test.cc -std=c++14 -g -o BlockingQueueTest; sudo gdb BlockingQueueTest; rm -r BlockingQueueTest *.dSYM

clean:
	rm BlockingQueueTest
