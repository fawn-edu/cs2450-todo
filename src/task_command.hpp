#pragma once

#include <chrono>
#include <cstddef>
#include <optional>
#include <string>
#include <utility>

#include "task_controller.hpp"

// Command interface: task actions are executed through one entry point.
class TaskCommand {
public:
    virtual ~TaskCommand() = default;
    virtual void execute(task_controller& controller) = 0;
};

struct TaskInput {
    std::string name;
    std::optional<char> priority;
    std::optional<std::chrono::sys_seconds> due;
};

// No index means create; an existing index means edit.
class SaveTaskCommand final : public TaskCommand {
public:
    SaveTaskCommand(TaskInput input, std::optional<std::size_t> index = std::nullopt)
        : input_(std::move(input)), index_(index) {}

    void execute(task_controller& controller) override {
        if (index_) {
            controller.edit_task(*index_, input_.name, input_.priority, input_.due);
        } else {
            controller.add_task(input_.name, input_.priority, input_.due);
        }
    }

private:
    TaskInput input_;
    std::optional<std::size_t> index_;
};

class DeleteTaskCommand final : public TaskCommand {
public:
    explicit DeleteTaskCommand(std::size_t index) : index_(index) {}

    void execute(task_controller& controller) override {
        controller.delete_task(index_);
    }

private:
    std::size_t index_;
};

class CompleteTaskCommand final : public TaskCommand {
public:
    CompleteTaskCommand(std::size_t index, bool completed)
        : index_(index), completed_(completed) {}

    void execute(task_controller& controller) override {
        controller.set_completed(index_, completed_);
    }

private:
    std::size_t index_;
    bool completed_;
};
