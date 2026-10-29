#pragma once

#include <expected>
#include <vector>
#include "task.hpp"

class task_controller {
public:
	enum class error { NoSuchTask, AmbiguousTask };
	task_controller(std::vector<task> &&t) : tasks{t} {}
	std::expected<task&, error> search_task(std::string_view name);
	void sort();
// private:
	std::vector<task> tasks;
};
