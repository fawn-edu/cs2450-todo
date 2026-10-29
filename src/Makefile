.POSIX:
.SUFFIXES:
.PHONY: clean test

CXX = c++
CXXFLAGS = -std=c++23 -O2 -Wall -Wextra -pedantic

test: todo.out
	rm -f todo.txt
	./todo.out create "(d) Test task 1"
	./todo.out complete "1"
	./todo.out create "(b) testt task  2"
	./todo.out complete "2"
	./todo.out reopen "2"
	./todo.out create "(c) Test task 3"
	./todo.out delete "3"
	./todo.out edit "1" "(a)"
	./todo.out edit "2" "Test task 2"
	./todo.out list
test-sort: todo.out
	rm -f todo.txt
	./todo.out create "No priority, incomplete 1"
	./todo.out create "No priority, complete 1"
	./todo.out create "(A) A priority, incomplete 1"
	./todo.out create "(A) A priority, complete 1"
	./todo.out create "(B) B priority, incomplete 1"
	./todo.out create "(B) B priority, complete 1"
	sleep 1s
	./todo.out create "No priority, incomplete 2"
	./todo.out create "No priority, complete 2"
	./todo.out create "(A) A priority, incomplete 2"
	./todo.out create "(A) A priority, complete 2"
	./todo.out create "(B) B priority, incomplete 2"
	./todo.out create "(B) B priority, complete 2"
	sleep 1s
	./todo.out complete "No priority, complete 1"
	./todo.out complete "A priority, complete 1"
	./todo.out complete "B priority, complete 1"
	sleep 1s
	./todo.out complete "No priority, complete 2"
	./todo.out complete "A priority, complete 2"
	./todo.out complete "B priority, complete 2"
	cat todo.txt
todo.out: main.cpp task.cpp task_controller.cpp task_view.cpp task.hpp task_controller.hpp task_view.hpp
	$(CXX) $(CXXFLAGS) -o $@ main.cpp task.cpp task_controller.cpp task_view.cpp
clean:
	rm -f todo.out* todo.txt *.pdb
