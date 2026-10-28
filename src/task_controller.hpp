#pragma once

#include <vector>
#include "task.hpp"

class task_controller {
public:
    int run(int argc, const char** argv);

private:
    std::vector<task> tasks;

    bool load_tasks();
    bool save_tasks();

    void print_usage(const char* argv0) const;
};
