#pragma once

#include <string>
#include <vector>
#include "task.hpp"

class task_view {
public:
	void show_usage(const char* argv0) const;
	void show_error(const std::string& message) const;
	void show_message(const std::string& message) const;
	void show_tasks(const std::vector<task>& tasks) const;
};
