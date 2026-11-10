#pragma once

#include <expected>
#include <optional>
#include <span>
#include <vector>

#include "task.hpp"

struct task_controller {
	enum class error {NoSuchTask, AmbiguousTask};

	task_controller(std::vector<task> &t);

	std::expected<task*, error> search_task(std::string_view name, auto cond);
	void add_task(std::string_view name, std::optional<char> priority);
	void edit_task(std::size_t index, std::string_view name, std::optional<char> priority);
	void delete_task(std::size_t index);
	void set_completed(std::size_t index, bool completed);
	std::span<const task> get_tasks() const;

private:
	std::vector<task>& tasks;
};
