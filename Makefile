CC=g++
CPPFLAGS=-g -Wall -std=c++11 -I.

default: final

final: Functions Student Database main
	$(CC) $(CPPFLAGS) -o bin/final libs/Functions.o libs/Student.o libs/Database.o libs/main.o

main: Functions Database Student
	$(CC) $(CPPFLAGS) -c src/main.cpp -o libs/main.o

Functions: Database Student
	$(CC) $(CPPFLAGS) -c src/Functions.cpp -o libs/Functions.o

Database: Student
	$(CC) $(CPPFLAGS) -c src/Database.cpp -o libs/Database.o

Student:  
	$(CC) $(CPPFLAGS) -c src/Student.cpp -o libs/Student.o

clean:
	rm libs/*.o 
