CC = g++
CFLAGS = -c -Wall -std=c++0x

all: simpleTest

gradingTest: gradingTest.o graphStudent.o
	$(CC) gradingTest.o graphStudent.o -o gradingTest

simpleTest: simpleTest.o graphStudent.o
	$(CC) simpleTest.o graphStudent.o -o simpleTest


simpleTest.o: simpleTest.cpp graph.h edge.h
	$(CC) $(CFLAGS) simpleTest.cpp

gradingTest.o: gradingTest.cpp graph.h edge.h
	$(CC) $(CFLAGS) gradingTest.cpp


graphStudent.o: graphStudent.cpp graph.h edge.h
	$(CC) $(CFLAGS) graphStudent.cpp


clean: 
	rm -rf *.o simpleTest gradingTest
