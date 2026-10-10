#include <cassert>
#include <format>
#include <fstream>
#include <iostream>
#include <iterator>
#include <print>
#include <string_view>
#include <vector>
#include "task.hpp"

static void print_usage() {
	// TODO
}

int main(int argc, const char **argv) {
	std::vector<task> tasks{}; // TODO #16: Load tasks from file; requires task::parse()
	// std::ifstream itodo{"todo.txt", std::ios::in};
	// if (!itodo.is_open()) {
	// 	std::println(std::cerr, "Could not open todo.txt for reading.");
	// 	return 1;
	// }
	// for (std::string line; getline(itodo, line);) tasks.emplace_back(task::parse(line));

	assert(argc > 1); // TODO: Print usage if not enough args
	std::string_view cmd{argv[1]};
	cmd.remove_prefix(std::min(cmd.find_first_not_of(" -/"), cmd.size()));

	if (cmd == "list" || cmd == "l") {
		// TODO #2 ezpz, please improve this if possible
		std::ostream_iterator<char> out{std::cout};
		for (const auto &task : tasks) std::format_to(out, "{}\n", task);
	} else if (cmd == "create" || cmd == "c") {
		// TODO: Reject duplicate task names for now
		assert(std::strlen(argv[2]) > 0); // TODO: Print usage if name empty
		// This will need actual parsing logic once tasks hold more
		// user-defined content than just a name
		tasks.emplace_back(task{.name = argv[2]});
	} else if (cmd == "edit" || cmd == "e") {
		assert(false); // TODO #3
		// Should expect a task name fragment in argv[2]
		// Exit with error if the name fragment is ambiguous
		// This should behave just like creating a new task, but the
		// default values of the new task are just the values of the
		// task being edited instead
	} else if (cmd == "complete" || cmd == "x") {
		assert(argc > 2);

		std::string_view fragment{argv[2]};
		task *match = nullptr;

		for (auto &t : tasks) {
			if (t.name.find(fragment) != std::string::npos) {
				if (match != nullptr) {
					std::println("Task name is ambiguous: {}", fragment);
					return 1;

				}
				match = &t;
			}
		}

		if (match == nullptr) {
			std::println("Task not found: {}", fragment);
			return 1;
		}

		match->completed =
			std::chrono::floor<std::chrono::seconds>(
				std::chrono::system_clock::now()
			);

	} else if (cmd == "reopen" || cmd == "o") {
		assert(argc > 2);

		std::string_view fragment{argv[2]};
		task *match = nullptr;

		for (auto &t : tasks) {
			if (t.name.find(fragment) != std::string::npos) {
				if (match != nullptr) {
					std::println("Task name is ambiguous: {}", fragment);
					return 1;
				}
				match = &t;
			}
		}

		if (match == nullptr) {
			std::println("Task not found: {}", fragment);
			return 1;
		}

		match->completed = std::nullopt;

	} else if (cmd == "delete") { // No shorthand, intentionally
		assert(false); // TODO #5
		// Should expect a task name fragment in argv[2]
		// Exit with error if the name fragment is ambiguous
		// Remove that task using erase()
		// https://en.cppreference.com/cpp/container/vector/erase
	} else if (cmd == "help" || cmd == "h" || cmd == "?") {
		print_usage();
	} else {
		std::println("\"{}\" is not a valid command", argv[1]);
		print_usage();
	}

	// Save tasks to todo.txt
	std::ofstream otodo{"todo.txt", std::ios::out | std::ios::trunc};
	if (!otodo.is_open()) {
		std::println(std::cerr, "Could not open todo.txt for writing.");
		return 1;
	}
	for (const auto &task : tasks) std::println(otodo, "{}\n", task);
	return 0;
}
