CC = g++
CFLAGS = -c -Wall -std=c++0x

all: simpleFinalTest

gradingFinalTest: gradingFinalTest.o wikiFinalStudent.o wikiFinal.o
	$(CC) gradingFinalTest.o wikiFinalStudent.o wikiFinal.o -o gradingFinalTest

simpleFinalTest: simpleFinalTest.o wikiFinalStudent.o wikiFinal.o
	$(CC) simpleFinalTest.o wikiFinalStudent.o wikiFinal.o -o simpleFinalTest


simpleFinalTest.o: simpleFinalTest.cpp wikiFinal.h
	$(CC) $(CFLAGS) simpleFinalTest.cpp wikiFinal.cpp

gradingFinalTest.o: gradingFinalTest.cpp wikiFinal.h
	$(CC) $(CFLAGS) gradingFinalTest.cpp


wikiFinalStudent.o: wikiFinalStudent.cpp wikiFinal.h
	$(CC) $(CFLAGS) wikiFinalStudent.cpp

wikiFinal.o: wikiFinal.cpp wikiFinal.cpp
	$(CC) $(CFLAGS) wikiFinal.cpp

clean: 
	rm -rf *.o simpleFinalTest gradingFinalTest
