#include <string>

#include <FL/Fl.H>
#include <FL/fl_ask.H>

#include "task_view.hpp"

task_view::task_view(task_controller &controller)
	: controller_(controller)
	, window_(1000, 700, "CS 2450 Task Manager")
	, new_task_button_(20, 20, 120, 30, "New Task")
	, edit_task_button_(150, 20, 120, 30, "Edit Task")
	, complete_task_button_(280, 20, 150, 30, "Complete / Reopen")
	, delete_task_button_(440, 20, 120, 30, "Delete Task")
	, tabs_(20, 70, 960, 600)
	, all_tab_(20, 95, 960, 575, "All")
	, incomplete_tab_(20, 95, 960, 575, "Incomplete")
	, complete_tab_(20, 95, 960, 575, "Complete")
	, all_tasks_(40, 120, 900, 500)
	, incomplete_tasks_(40, 120, 900, 500)
	, complete_tasks_(40, 120, 900, 500) {
	new_task_button_.callback(new_task_cb, this);
	edit_task_button_.callback(edit_task_cb, this);
	complete_task_button_.callback(complete_task_cb, this);
	all_tasks_.callback(selection_cb, this);
	incomplete_tasks_.callback(selection_cb, this);
	complete_tasks_.callback(selection_cb, this);
	delete_task_button_.callback(delete_task_cb, this);

	// Move each browser into its corresponding tab
	all_tab_.add(all_tasks_);
	incomplete_tab_.add(incomplete_tasks_);
	complete_tab_.add(complete_tasks_);

	// Make sure all three tabs belong to the tab container
	tabs_.add(all_tab_);
	tabs_.add(incomplete_tab_);
	tabs_.add(complete_tab_);

	// Make sure the tabs container belongs to the window
	window_.add(tabs_);

	all_tab_.end();
	incomplete_tab_.end();
	complete_tab_.end();
	tabs_.end();

	tabs_.value(&all_tab_);

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

	all_task_indices_.clear();
	incomplete_task_indices_.clear();
	complete_task_indices_.clear();

	const auto &tasks = controller_.get_tasks();

	for (size_t i = 0; i < tasks.size(); i++) {
		const auto &task = tasks[i];
		// for (const auto &task : controller_.get_tasks()) {
		std::string text;

		// Uncomment if you want to display completion status in the task list. Looks a lil funky
		// with the priority display, so I commented it out for now.

		// if (task.completed.has_value()) {
		// 	text += "[x] ";
		// } else {
		// 	text += "[ ] ";
		// }

		if (task.priority.has_value()) {
			text += "[";
			text += task.priority.value();
			text += "] ";
		} else {
			text += "[ ] ";
		}

		text += task.name;

		all_tasks_.add(text.c_str());
		all_task_indices_.push_back(i);

		if (task.completed.has_value()) {
			complete_tasks_.add(text.c_str());
			complete_task_indices_.push_back(i);
		} else {
			incomplete_tasks_.add(text.c_str());
			incomplete_task_indices_.push_back(i);
		}
	}
}

void task_view::new_task_cb(Fl_Widget *, void *data) {
	auto *view = static_cast<task_view *>(data);

	// Get task name
	const char *name = fl_input("Enter task name:");

	if (name == nullptr) {
		return;
	}

	std::string task_name = name;

	if (task_name.empty()) {
		fl_alert("Task name cannot be empty.");
		return;
	}

	// Get optional priority
	const char *priority = fl_input("Enter priority (A-Z), or leave blank:");

	if (priority == nullptr) {
		return;
	}

	std::optional<char> task_priority = std::nullopt;

	if (priority[0] != '\0') {
		char letter = priority[0];

		if (priority[1] != '\0' || letter < 'A' || letter > 'Z') {
			fl_alert("Priority must be a single uppercase letter (A-Z).");
			return;
		}

		task_priority = letter;
	}

	// Create task
	view->controller_.add_task(task_name, task_priority);
	view->refresh_lists();
}

std::optional<size_t> task_view::selected_task_index() {
	const Fl_Hold_Browser *browser = nullptr;
	const std::vector<size_t> *indices = nullptr;

	if (tabs_.value() == &all_tab_) {
		browser = &all_tasks_;
		indices = &all_task_indices_;
	} else if (tabs_.value() == &incomplete_tab_) {
		browser = &incomplete_tasks_;
		indices = &incomplete_task_indices_;
	} else if (tabs_.value() == &complete_tab_) {
		browser = &complete_tasks_;
		indices = &complete_task_indices_;
	} else {
		return std::nullopt;
	}

	int row = browser->value();

	if (row < 1 || static_cast<size_t>(row) > indices->size()) {
		return std::nullopt;
	}

	return (*indices)[row - 1];
}

void task_view::selection_cb(Fl_Widget *, void *data) {
	auto *view = static_cast<task_view *>(data);

	auto index = view->selected_task_index();
}

void task_view::edit_task_cb(Fl_Widget *, void *data) {
	auto *view = static_cast<task_view *>(data);

	// Get selected task
	auto index = view->selected_task_index();

	if (!index.has_value()) {
		fl_alert("Please select a task to edit.");
		return;
	}

	// Get current task information
	const auto &task = view->controller_.get_tasks()[index.value()];

	// Ask for updated name
	const char *name = fl_input("Edit task name:", task.name.c_str());

	if (name == nullptr) {
		return;
	}

	std::string new_name = name;

	if (new_name.empty()) {
		fl_alert("Task name cannot be empty.");
		return;
	}

	// Get current priority
	std::string current_priority;

	if (task.priority.has_value()) {
		current_priority = task.priority.value();
	}

	// Ask for updated priority
	const char *priority =
		fl_input("Edit priority (A-Z), or leave blank:", current_priority.c_str());

	if (priority == nullptr) {
		return;
	}

	std::optional<char> new_priority = std::nullopt;

	if (priority[0] != '\0') {
		char letter = priority[0];

		if (priority[1] != '\0' || letter < 'A' || letter > 'Z') {
			fl_alert("Priority must be a single uppercase letter (A-Z).");
			return;
		}

		new_priority = letter;
	}

	// Update task through controller
	view->controller_.edit_task(index.value(), new_name, new_priority);

	// Refresh displayed tasks
	view->refresh_lists();
}

void task_view::complete_task_cb(Fl_Widget *, void *data) {
	auto *view = static_cast<task_view *>(data);

	// Get selected task
	auto index = view->selected_task_index();

	if (!index.has_value()) {
		fl_alert("Please select a task first.");
		return;
	}

	// Check current completion status
	const auto &task = view->controller_.get_tasks()[index.value()];

	bool is_completed = task.completed.has_value();

	// Toggle completion status
	view->controller_.set_completed(index.value(), !is_completed);

	// Refresh the lists
	view->refresh_lists();
}

void task_view::delete_task_cb(Fl_Widget *, void *data) {
	auto *view = static_cast<task_view *>(data);

	// Get selected task
	auto index = view->selected_task_index();

	if (!index.has_value()) {
		fl_alert("Please select a task to delete.");
		return;
	}

	// Ask for confirmation
	int confirmation =
		fl_choice("Are you sure you want to delete this task?", "Cancel", "Delete", nullptr);

	// Cancel deletion
	if (confirmation != 1) {
		return;
	}

	// Delete the selected task
	view->controller_.delete_task(index.value());

	// Refresh the task lists
	view->refresh_lists();
}