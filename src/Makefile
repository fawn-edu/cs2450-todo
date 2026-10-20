.POSIX:
.SUFFIXES:
.PHONY: clean test

CXX = c++
CXXFLAGS = -std=c++23 -O2 -Wall -Wextra -pedantic

test: todo.out
	./todo.out create "(A) Test task 1"
	./todo.out complete "1"
	./todo.out reopen "1"
	./todo.out create "(B) Test task 2"
	./todo.out complete "2"
	./todo.out create "(c) Test task 3"
	./todo.out delete "3"
	./todo.out list
todo.out: main.cpp task.cpp task.hpp
	$(CXX) $(CXXFLAGS) -o $@ main.cpp task.cpp
clean:
	rm -f todo.out* todo.txt *.pdb
