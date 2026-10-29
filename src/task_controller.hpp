#pragma once

#include <vector>
#include "task.hpp"
#include "task_view.hpp"

class task_controller {
public:
	int run(int argc, const char** argv);

private:
	std::vector<task> tasks;
	task_view view;

	bool load_tasks();
	bool save_tasks();
};
