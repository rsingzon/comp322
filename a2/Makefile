CC = g++
CFLAGS = -c -Wall -std=c++0x

all: simpleWikiTest

gradingWikiTest: gradingWikiTest.o wikiGraphStudent.o
	$(CC) gradingWikiTest.o wikiGraphStudent.o -o gradingWikiTest

simpleWikiTest: simpleWikiTest.o wikiGraphStudent.o
	$(CC) simpleWikiTest.o wikiGraphStudent.o -o simpleWikiTest


simpleWikiTest.o: simpleWikiTest.cpp graphSTL.h
	$(CC) $(CFLAGS) simpleWikiTest.cpp

gradingWikiTest.o: gradingWikiTest.cpp graphSTL.h
	$(CC) $(CFLAGS) gradingWikiTest.cpp


wikiGraphStudent.o: wikiGraphStudent.cpp graphSTL.h
	$(CC) $(CFLAGS) wikiGraphStudent.cpp


clean: 
	rm -rf *.o simpleWikiTest gradingWikiTest
