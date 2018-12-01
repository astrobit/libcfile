all: bin/gccshared bin/clangshared

bin/gccshared: src/shared_file_test.cpp ../include/cfile.hpp ../libcfile.la
	g++ --std=c++11 -I../include -c src/shared_file_test.cpp -o src/shared_file_test.gcc.o
	libtool --mode=link g++ --std=c++11 -g -O -o bin/gccshared src/shared_file_test.gcc.o ../libcfile.la


bin/clangshared: src/shared_file_test.cpp ../include/cfile.hpp ../libcfile.la
	CXX=clang++
	clang++ --std=c++11 -I../include -c src/shared_file_test.cpp -o src/shared_file_test.clang.o
	libtool --mode=link --tag=CXX clang++ --std=c++11 -g -O -o bin/clangshared src/shared_file_test.clang.o ../libcfile.la


clean:
	rm bin/*
	rm src/*.o
	
