#include <fstream>
#include <iostream>
#include <print>
#include <vector>

#include "task.hpp"
#include "task_controller.hpp"
#include "task_view.hpp"

int main() {
	// Load tasks from todo.txt
	std::vector<task> tasks{};
	std::ifstream itodo{"todo.txt", std::ios::in | std::ios::app};
	if (!itodo.is_open()) {
		std::println(std::cerr, "could not open todo.txt for reading");
		return 1;
	}
	auto linenr = 1u;
	for (std::string line; getline(itodo, line); ++linenr) {
		try {
			tasks.emplace_back(task::parse(line));
		} catch (std::invalid_argument &e) {
			std::println(std::cerr, "syntax error in todo.txt on line {}: {}", linenr, e.what());
			return 1;
		}
	}

	task_controller c{tasks};
	task_view{c}.run();

	// Save tasks to todo.txt
	std::ofstream otodo{"todo.txt", std::ios::out | std::ios::trunc};
	if (!otodo.is_open()) {
		std::println(std::cerr, "could not open todo.txt for writing");
		return 1;
	}
	for (const auto &task : tasks) std::println(otodo, "{}", task);
	return 0;
}
