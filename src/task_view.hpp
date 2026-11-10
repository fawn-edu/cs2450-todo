#pragma once

#include <FL/Fl_Button.H>
#include <FL/Fl_Hold_Browser.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Window.H>

#include "task_controller.hpp"

class task_view {
public:
	explicit task_view(task_controller &controller);
	void run();

private:
	static void new_task_cb(Fl_Widget *widget, void *data);
	void refresh_lists();

	task_controller &controller_;
	Fl_Window window_;
	Fl_Button new_task_button_;
	Fl_Tabs tabs_;
	Fl_Group all_tab_, incomplete_tab_, complete_tab_;
	Fl_Hold_Browser all_tasks_, incomplete_tasks_, complete_tasks_;
};
