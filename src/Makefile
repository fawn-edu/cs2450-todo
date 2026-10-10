.POSIX:
.SUFFIXES:
.PHONY: clean test

CXX = c++
CXXFLAGS = -std=c++23 -O2 -Wall -Wextra -pedantic

test: todo.out
	./todo.out create "Test task"
todo.out: main.cpp task.cpp task.hpp
	$(CXX) $(CXXFLAGS) -o $@ main.cpp task.cpp
clean:
	rm -f todo.out* todo.txt *.pdb
