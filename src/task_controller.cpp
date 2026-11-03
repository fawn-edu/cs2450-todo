#include "task_controller.hpp"

using std::expected, std::unexpected, std::string_view;

task_controller::task_controller(std::vector<task> &t) : tasks{t} {}

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
