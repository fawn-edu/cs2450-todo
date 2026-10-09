.POSIX:
.SUFFIXES:
.PHONY: clean test

CXX = c++
CXXFLAGS = -std=c++23 -O2 -Wall -Wextra -pedantic

test: todo.out
	./todo.out
todo.out: main.cpp task.hpp
	$(CXX) $(CXXFLAGS) -o $@ main.cpp
clean:
	rm -f todo.out* *.pdb
