#include "task_controller.hpp"

#include <algorithm>
#include <cctype>

using std::expected;
using std::string_view;
using std::unexpected;

task_controller::task_controller(std::vector<task>& t)
	: tasks{t}
{
}

expected<task*, task_controller::error>
task_controller::search_task(string_view name, auto cond)
{
	task* match = nullptr;

	for (auto& t : tasks) {
		if (!cond(t) || !t.name.contains(name))
			continue;

		if (match)
			return unexpected{error::AmbiguousTask};

		match = &t;
	}

	if (!match)
		return unexpected{error::NoSuchTask};

	return match;
}

void task_controller::add_task(
	const std::string& name,
	std::optional<char> priority)
{
	task t;

	t.name = name.empty() ? "New task" : name;
	t.priority = priority;

	tasks.push_back(std::move(t));

	std::ranges::sort(tasks);
}

void task_controller::edit_task(
	std::size_t index,
	const std::string& name,
	std::optional<char> priority)
{
	if (index >= tasks.size())
		return;

	tasks[index].name = name.empty()
		? "New task"
		: name;

	tasks[index].priority = priority;

	std::ranges::sort(tasks);
}

void task_controller::delete_task(std::size_t index)
{
	if (index >= tasks.size())
		return;

	tasks.erase(tasks.begin() + index);
}

void task_controller::set_completed(
	std::size_t index,
	bool completed)
{
	if (index >= tasks.size())
		return;

	if (completed) {
		tasks[index].complete();
	}
	else {
		tasks[index].completed.reset();
	}

	std::ranges::sort(tasks);
}

const std::vector<task>&
task_controller::get_tasks() const
{
	return tasks;
}
