CC = g++
CFLAGS = -I../include -Wall --std=c++17

all:
	${CC} ${CFLAGS} tests.cpp -o tests

test:
	${CC} ${CFLAGS} tests.cpp -o tests
	./tests

clean:
	rme tests.cpp test_maps.cpp test_numerics.cpp test_vectors.cpp Makefile
