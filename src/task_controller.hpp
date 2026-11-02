#pragma once

#include <expected>
#include <vector>
#include "task.hpp"

struct task_controller {
	enum class error { NoSuchTask, AmbiguousTask };
	task_controller(std::vector<task> &t);
	std::expected<task*, error> search_task(std::string_view name, auto cond);
// private:
	// task_view view;
	std::vector<task> &tasks;
};
