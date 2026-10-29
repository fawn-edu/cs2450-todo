#include <format>
#include <iostream>
#include <print>
#include "task_view.hpp"

void task_view::show_usage(const char* argv0) const {
	std::print(
		"Usage:\n"
		"\t{} list			   List all tasks\n"
		"\t{} create NAME	   Create a new task named NAME\n"
		"\t{} complete NAME	   Mark task named NAME as complete\n"
		"\t{} reopen NAME	   Mark task named NAME as incomplete\n"
		"\t{} edit NAME EDIT   Modify task named NAME with EDIT\n"
		"\t{} delete NAME	   Delete task named NAME\n"
		"\t{} help			   Show program usage\n",
		argv0, argv0, argv0, argv0,
		argv0, argv0, argv0
	);
}

void task_view::show_error(const std::string& message) const {
	std::println(std::cerr, "{}", message);
}

void task_view::show_message(const std::string& message) const {
	std::println("{}", message);
}

void task_view::show_tasks(const std::vector<task>& tasks) const {
	for (const auto& t : tasks) {
		const auto priority =
			t.priority
			? std::format("({}) ", t.priority.value())
			: "";

		std::println(
			"[{}] {}{}",
			t.completed ? 'x' : ' ',
			priority,
			t.name
		);
	}
}
