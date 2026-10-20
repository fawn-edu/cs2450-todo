#include <cassert>
#include <format>
#include <fstream>
#include <iostream>
#include <print>
#include <string_view>
#include <vector>
#include "task.hpp"

static void print_usage(const char *argv0) {
	std::print(
		"Usage:\n"
		"\t{} list             List all tasks\n"
		"\t{} create NAME      Create a new task named NAME\n"
		"\t{} complete NAME    Mark task named NAME as complete\n"
		"\t{} reopen NAME      Mark task named NAME as incomplete\n"
		"\t{} delete NAME      Delete task named NAME\n"
		"\t{} help             Show program usage\n",
		argv0, argv0, argv0, argv0, argv0, argv0
	);
}

int main(int argc, const char **argv) {
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

	if (argc < 2) {
		std::println(std::cerr, "no arguments provided");
		print_usage(argv[0]);
		return 1;
	}
	std::string_view cmd{argv[1]};
	cmd.remove_prefix(std::min(cmd.find_first_not_of(" -/"), cmd.size()));

	if (cmd == "list" || cmd == "l") {
		for (const auto &t : tasks) {
			const auto priority = t.priority
				? std::format("({}) ", t.priority.value())
				: "";
			std::println("[{}] {}{}",
				t.completed ? 'x' : ' ',
				priority,
				t.name
			);
		}
	} else if (cmd == "create" || cmd == "c") {
		if (argc < 3 || !std::strlen(argv[2])) {
			std::println(std::cerr, "{} requires task name", argv[1]);
			return 1;
		}
		// TODO: Factor this out into a "lite" parse function that does
		// not expect dates, have the full parse function call into that
		// same function once dates are parsed
		auto input = std::string_view{argv[2]};
		std::optional<char> priority = std::nullopt;
		if (input.starts_with('(')) {
			const auto end = input.find_first_of(' ');
			if (end == std::string_view::npos) {
				std::println(std::cerr, "Task beginning with parentheses expects priority");
				return 1;
			}
			const auto raw = input.substr(0, end);
			priority = raw.at(1); // Inside parentheses
			if ('z' < priority || priority < 'A') {
				std::println(std::cerr, "Task priority must be alphabetic");
				return 1;
			}
			input.remove_prefix(end + 1); // +1 for trailing space immediately after end
		}
		tasks.emplace_back(task{.name = std::string{input}, .priority = priority});
	// } else if (cmd == "edit" || cmd == "e") { // TODO #3
		// Should expect a task name fragment in argv[2]
		// Exit with error if the name fragment is ambiguous
		// This should behave just like creating a new task, but the
		// default values of the new task are just the values of the
		// task being edited instead
	} else if (cmd == "complete" || cmd == "x") {
		if (argc < 3) {
			std::println(std::cerr, "{} requires task name", argv[1]);
			return 1;
		}

		task *match = nullptr;
		for (auto &t : tasks) {
			// Ignore already completed tasks
			if (t.completed || !t.name.contains(argv[2])) continue;
			if (match) {
				std::println(std::cerr, "\"{}\" is ambiguous", argv[2]);
				return 1;
			}
			match = &t;
		}

		if (!match) {
			std::println(std::cerr, "\"{}\" does not match any incomplete tasks", argv[2]);
			return 1;
		}
		match->completed = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
	} else if (cmd == "reopen" || cmd == "o") {
		if (argc < 3) {
			std::println(std::cerr, "{} requires task name", argv[1]);
			return 1;
		}

		task *match = nullptr;
		for (auto &t : tasks) {
			// Ignore incomplete tasks
			if (!t.completed || !t.name.contains(argv[2])) continue;
			if (match) {
				std::println(std::cerr, "\"{}\" is ambiguous", argv[2]);
				return 1;
			}
			match = &t;
		}

		if (!match) {
			std::println(std::cerr, "\"{}\" does not match any complete tasks", argv[2]);
			return 1;
		}
		match->completed = std::nullopt;
	} else if (cmd == "delete") { // No shorthand, intentionally
		if (argc < 3) {
			std::println(std::cerr, "{} requires task name", argv[1]);
			return 1;
		}

		task *match = nullptr;
		for (task &t : tasks) {
			if (!t.name.contains(argv[2])) continue;
			if (match) {
				std::println(std::cerr, "\"{}\" is ambiguous", argv[2]);
				return 1;
			}
			match = &t;
		}

		if (!match) {
			std::println(std::cerr, "\"{}\" does not match any tasks", argv[2]);
			return 1;
		}
		// I hate C++. Just let me do pointer arithmetic to calculate
		// the index of the match ffs
		tasks.erase(tasks.begin() + std::distance(tasks.data(), match));
	} else if (cmd == "help" || cmd == "h" || cmd == "?") {
		print_usage(argv[0]);
	} else {
		std::println("\"{}\" is not a valid command", argv[1]);
		print_usage(argv[0]);
	}

	// Save tasks to todo.txt
	// TODO: Sort tasks by created date, then sort completed tasks to
	// bottom
	std::ofstream otodo{"todo.txt", std::ios::out | std::ios::trunc};
	if (!otodo.is_open()) {
		std::println(std::cerr, "could not open todo.txt for writing");
		return 1;
	}
	for (const auto &task : tasks) std::println(otodo, "{}", task);
	return 0;
}
