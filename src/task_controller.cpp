#include "task_controller.hpp"

#include <algorithm>

using std::expected;
using std::string_view;
using std::unexpected;

task_controller::task_controller(std::vector<task> &t)
	: tasks{t}
{}

expected<task*, task_controller::error> task_controller::search_task(string_view name, auto cond) {
	task *match = nullptr;
	for (auto &t : tasks) {
		if (!cond(t) || !t.name.contains(name)) continue;
		if (match) return unexpected{error::AmbiguousTask};
		match = &t;
	}
	if (!match) return unexpected{error::NoSuchTask};
	return match;
}

void task_controller::add_task(
	std::string_view name,
	std::optional<char> priority,
	std::optional<std::chrono::sys_seconds> due
) {
	tasks.emplace_back(task{
		.priority = priority,
		.due = due,
		.name = name.empty() ? "New task" : std::string{name},
	});
	std::ranges::sort(tasks);
}

void task_controller::edit_task(
	std::size_t index,
	std::string_view name,
	std::optional<char> priority,
	std::optional<std::chrono::sys_seconds> due
) {
	if (index >= tasks.size()) return;
	tasks[index].priority = priority;
	tasks[index].due = due;
	tasks[index].name = name.empty()
		? "New task"
		: name;
	std::ranges::sort(tasks);
}

void task_controller::delete_task(std::size_t index) {
	if (index >= tasks.size()) return;
	tasks.erase(tasks.begin() + static_cast<std::ptrdiff_t>(index));
}

void task_controller::set_completed(std::size_t index, bool completed) {
	if (index >= tasks.size()) return;
	if (completed) {
		tasks[index].complete();
	} else {
		tasks[index].completed.reset();
	}
	std::ranges::sort(tasks);
}

std::span<const task> task_controller::get_tasks() const { return tasks; }
