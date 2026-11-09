#include "task_view.hpp"

#include <FL/Fl.H>

task_view::task_view(task_controller& controller)
    : controller_(controller),

      window_(1000, 700, "Task Manager"),

      new_task_button_(20, 20, 120, 30, "New Task"),

      tabs_(20, 70, 960, 600),

      all_tab_(20, 95, 960, 575, "All"),
      incomplete_tab_(20, 95, 960, 575, "Incomplete"),
      complete_tab_(20, 95, 960, 575, "Complete"),

      all_tasks_(40, 120, 900, 500),
      incomplete_tasks_(40, 120, 900, 500),
      complete_tasks_(40, 120, 900, 500)
{
    new_task_button_.callback(
        new_task_cb,
        this);

    all_tab_.end();
    incomplete_tab_.end();
    complete_tab_.end();

    tabs_.end();

    refresh_lists();

    window_.end();
}

void task_view::run()
{
    window_.show();

    Fl::run();
}
