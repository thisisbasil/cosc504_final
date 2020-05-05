CC=g++
CPPFLAGS=-g -w -std=c++11 -I.

default: final

final: Functions Student Database main
	$(CC) $(CPPFLAGS) -o bin/final objs/Functions.o objs/Student.o objs/Database.o objs/main.o

main: Functions Database Student
	$(CC) $(CPPFLAGS) -c src/main.cpp -o objs/main.o

Functions: Database Student
	$(CC) $(CPPFLAGS) -c src/Functions.cpp -o objs/Functions.o

Database: Student
	$(CC) $(CPPFLAGS) -c src/Database.cpp -o objs/Database.o

Student:  
	$(CC) $(CPPFLAGS) -c src/Student.cpp -o objs/Student.o

clean:
	for file in objs/* bin/*; do \
		if [ -e "$file" ]; then \
			rm "$$file" || exit 1; \
		fi \
	done	
