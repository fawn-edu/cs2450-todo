#include <cctype>
#include <string>

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Return_Button.H>
#include <FL/fl_ask.H>
#include <FL/fl_draw.H>

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
		std::string text{task.completed ? "[x] " : "[ ] "};
		if (task.priority)
			text += std::format("({}) ", *task.priority);
		text += task.name;
		if (task.due) {
			text += std::format(" ({}: {:%F %TZ})",
								!task.completed && std::chrono::floor<std::chrono::seconds>(
													   std::chrono::system_clock::now()) > *task.due
									? "overdue"
									: "due",
								*task.due // TODO: Display local time
			);
		}
		all_tasks_.add(text.c_str());
		all_task_indices_.emplace_back(i);

		if (task.completed.has_value()) {
			complete_tasks_.add(text.c_str());
			complete_task_indices_.emplace_back(i);
		} else {
			incomplete_tasks_.add(text.c_str());
			incomplete_task_indices_.emplace_back(i);
		}
	}
}

static std::optional<std::chrono::sys_seconds> parse_due_date(std::string_view text) {
	using namespace std::chrono;

	if (text.size() != 10 || text[4] != '-' || text[7] != '-') {
		return std::nullopt;
	}

	for (std::size_t i = 0; i < text.size(); ++i) {
		if (i == 4 || i == 7)
			continue;
		if (text[i] < '0' || text[i] > '9')
			return std::nullopt;
	}

	const year_month_day date = year{std::stoi(std::string{text.substr(0, 4)})} /
								std::stoi(std::string{text.substr(5, 2)}) /
								std::stoi(std::string{text.substr(8, 2)});

	if (!date.ok() || date.year() < year{1})
		return std::nullopt;

	return sys_days{date} + hours{24} - seconds{1};
}

void task_view::new_task_cb(Fl_Widget *, void *data) {
	auto *view = static_cast<task_view *>(data);
	view->show_task_dialog();
}

void task_view::show_task_dialog(std::optional<std::size_t> index) {
	Fl_Window dialog(500, 238, index ? "Edit Task" : "New Task");

	Fl_Input name(145, 20, 335, 28, "Task name");

	Fl_Input priority(145, 58, 335, 28, "Priority (A-Z)");
	Fl_Box priority_hint(145, 86, 335, 18, "Optional, single letter (A-Z)");
	priority_hint.align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);

	Fl_Input due(145, 114, 335, 28, "Due date (UTC)");
	Fl_Box hint(145, 146, 335, 18, "YYYY-MM-DD, optional, end of UTC day");
	hint.align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);

	Fl_Box error(20, 168, 460, 18);
	error.align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
	error.labelcolor(FL_RED);

	Fl_Button cancel(270, 194, 100, 28, "Cancel");
	Fl_Return_Button create(380, 194, 100, 28, index ? "Save" : "Create");
	dialog.end();
	dialog.set_modal();

	std::optional<std::chrono::sys_seconds> original_due;
	std::string original_due_text;

	if (index) {
		const auto &current = controller_.get_tasks()[*index];

		name.value(current.name.c_str());

		if (current.priority) {
			const char letter[] = {*current.priority, '\0'};
			priority.value(letter);
		}

		original_due = current.due;
		if (original_due) {
			original_due_text = std::format("{:%F}", *original_due);
			due.value(original_due_text.c_str());
		}
	}

	bool submitted = false;
	create.callback([](Fl_Widget *, void *data) { *static_cast<bool *>(data) = true; }, &submitted);

	cancel.callback([](Fl_Widget *, void *data) { static_cast<Fl_Window *>(data)->hide(); },
					&dialog);

	dialog.show();
	name.take_focus();

	while (dialog.shown()) {
		Fl::wait();
		if (!dialog.shown() || !submitted)
			continue;
		submitted = false;

		const std::string_view task_name = name.value();
		const std::string_view priority_text = priority.value();
		const std::string_view due_text = due.value();

		if (task_name.find_first_not_of(" \t") == std::string_view::npos) {
			error.label("Enter a task name.");
			name.take_focus();
			continue;
		}

		std::optional<char> task_priority;
		if (!priority_text.empty()) {
			const char letter = priority_text.front();
			const bool is_letter =
				(letter >= 'A' && letter <= 'Z') || (letter >= 'a' && letter <= 'z');

			if (priority_text.size() != 1 || !is_letter) {
				error.label("Priority must be a single letter (A-Z).");
				priority.take_focus();
				continue;
			}

			task_priority = static_cast<char>(std::toupper(letter));
		}

		std::optional<std::chrono::sys_seconds> task_due;

		if (due_text == original_due_text) {
			task_due = original_due;
		} else if (!due_text.empty()) {
			task_due = parse_due_date(due_text);
			if (!task_due) {
				error.label("Enter a valid due date in YYYY-MM-DD format.");
				due.take_focus();
				continue;
			}
		}

		if (index) {
			controller_.edit_task(*index, task_name, task_priority, task_due);
		} else {
			controller_.add_task(task_name, task_priority, task_due);
		}

		refresh_lists();
		dialog.hide();
	}
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
	const auto index = view->selected_task_index();

	if (!index) {
		fl_alert("Please select a task to edit.");
		return;
	}

	view->show_task_dialog(index);
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
