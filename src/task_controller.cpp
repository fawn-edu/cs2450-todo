#include "task_controller.hpp"

#include <algorithm>
#include <cassert>
#include <format>
#include <fstream>
#include <iostream>
#include <print>
#include <string_view>
#include <vector>
#include "task.hpp"
#include "cstring"

void task_controller::print_usage(const char *argv0) const {
    std::cout << std::format(
        "Usage:\n"
        "\t{} list             List all tasks\n"
        "\t{} create NAME      Create a new task named NAME\n"
        "\t{} complete NAME    Mark task named NAME as complete\n"
        "\t{} reopen NAME      Mark task named NAME as incomplete\n"
        "\t{} edit NAME EDIT   Modify task named NAME with EDIT\n"
        "\t{} delete NAME      Delete task named NAME\n"
        "\t{} help             Show program usage\n",
        argv0, argv0, argv0,
        argv0, argv0, argv0,
        argv0
    );
}

bool task_controller::load_tasks() {
    std::ifstream itdodo{"todo.txt", std::ios::in | std::ios::app};

    if (!itdodo.is_open()) {
        std::cerr << "couldn't open todo.txt for reading";
        return false;
    }

    auto linenr = 1u;

    for (std::string line; getline(itdodo, line); ++linenr) {
        try {
            tasks.emplace_back(task::parse(line));
        }
        catch (std::invalid_argument& e) {
            std::cerr << "syntax error in todo.txt on line " << linenr << ": " << e.what() << std::endl;
            return false;
        }
    }
    return true;
}

bool task_controller::save_tasks() {
    std::ranges::sort(tasks);
    std::ofstream todo_file{
      "todo.txt", std::ios::out | std::ios::trunc
    };

    if (!todo_file.is_open()) {
        std::cerr << "couldn't open todo.txt for writing\n";
        return false;
    }

    for (const auto& task : tasks) {
        todo_file << std::format("{}\n", task);
    }
    return true;
}

int task_controller::run(int argc, const char** argv) {
    if (!load_tasks()) {
        return 1;
    }

    if (argc < 2) {
        std::println(std::cerr, "no argument provided");
        print_usage(argv[0]);
        return 1;
    }

    std::string_view cmd{argv[1]};
    cmd.remove_prefix(std::min(cmd.find_first_not_of(" -/"), cmd.size())
        );

    if (cmd == "help" || cmd == "h" || cmd == "?") {
        print_usage(argv[0]);
    } else if (cmd == "list" || cmd == "l") {
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
		try {
			auto t = task{}.edit(argv[2]);
			if (t.name.empty()) throw std::invalid_argument{"empty name"};
			tasks.emplace_back(t);
		} catch (std::invalid_argument &e) {
			std::println(std::cerr, "syntax error in task: {}", e.what());
			return 1;
		}
	} else if (cmd == "edit" || cmd == "e") {
		if (argc < 4) {
			std::println(std::cerr, "{} requires task name and edits", argv[1]);
			return 1;
		}

		task *match = nullptr;
		for (auto &t : tasks) {
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

		try {
			match->edit(argv[3]);
		} catch (std::invalid_argument &e) {
			std::println(std::cerr, "syntax error in edit: {}", e.what());
			return 1;
		}
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
		tasks.erase(tasks.begin() + std::distance(tasks.data(), match));
	} else if (cmd == "help" || cmd == "h" || cmd == "?") {
		print_usage(argv[0]);
	} else {
		std::println("\"{}\" is not a valid command", argv[1]);
		print_usage(argv[0]);
	}

    save_tasks();
    return 0;
}
