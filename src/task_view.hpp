#pragma once

#include <FL/Fl_Button.H>
#include <FL/Fl_Hold_Browser.H>
#include <FL/Fl_Hold_Browser.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Window.H>

#include "task_controller.hpp"

class task_view {
public:
    explicit task_view(task_controller& controller);

    void run();

private:
    task_controller& controller_;

    Fl_Window window_;

    Fl_Button new_task_button_;

    Fl_Tabs tabs_;

    Fl_Group all_tab_;
    Fl_Group incomplete_tab_;
    Fl_Group complete_tab_;

    Fl_Hold_Browser all_tasks_;
    Fl_Hold_Browser incomplete_tasks_;
    Fl_Hold_Browser complete_tasks_;

    void refresh_lists();

    static void new_task_cb(
        Fl_Widget* widget,
        void* data);
};
