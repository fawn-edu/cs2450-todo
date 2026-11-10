#include <string>

#include <FL/Fl.H>

#include "task_view.hpp"

task_view::task_view(task_controller& controller)
	: controller_(controller)
	, window_(1000, 700, "CS 2450 Task Manager")
	, new_task_button_(20, 20, 120, 30, "New Task")
	, tabs_(20, 70, 960, 600)
	, all_tab_(20, 95, 960, 575, "All")
	, incomplete_tab_(20, 95, 960, 575, "Incomplete")
	, complete_tab_(20, 95, 960, 575, "Complete")
	, all_tasks_(40, 120, 900, 500)
	, incomplete_tasks_(40, 120, 900, 500)
	, complete_tasks_(40, 120, 900, 500)
{
	new_task_button_.callback(new_task_cb, this);
	all_tab_.end();
	incomplete_tab_.end();
	complete_tab_.end();
	tabs_.end();
	refresh_lists();
	window_.end();
}

void task_view::run() {
	window_.show();
	Fl::run();
}

void task_view::refresh_lists() {
	all_tasks_.clear();
	incomplete_tasks_.clear();
	complete_tasks_.clear();

	for (const auto &task : controller_.get_tasks()) {
		std::string text;

		if (task.priority.has_value()) {
			text += "[";
			text += task.priority.value();
			text += "] ";
		} else {
			text += "[ ] ";
		}

		text += task.name;

		all_tasks_.add(text.c_str());

		if (task.completed.has_value()) {
			complete_tasks_.add(text.c_str());
		} else {
			incomplete_tasks_.add(text.c_str());
		}
	}
}

void task_view::new_task_cb(Fl_Widget*, void *data) {
	auto *view = static_cast<task_view*>(data);
	view->controller_.add_task("New Task", std::nullopt);
	view->refresh_lists();
}
