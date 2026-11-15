#pragma once

#include <chrono>
#include <compare>
#include <string>
#include <optional>

/// @brief An individual todo-list task
struct task {
	/// @brief Parses a task from a `string_view`
	/// @throws std::invalid_argument if @p serialized is not a valid
	/// text representation of a @ref task
	/// @see https://github.com/todotxt/todo.txt#todotxt-format-rules
	static task parse(std::string_view serialized);

	/// @brief Sets this task's completed date to the current datetime
	/// @details This method has no effect if this task is completed
	/// already.
	task &complete();

	/// @brief Orders two tasks based on contents
	/// @details Orders tasks using the following criteria:
	/// - Incomplete tasks are ordered before completed tasks.
	/// - Completed tasks are ordered by their completed date.
	/// - Incomplete tasks with a due date are ordered before incomplete
	///   tasks without one.
	/// - Two incomplete tasks with due dates are ordered by their due
	///   dates.
	/// - Incomplete tasks with a priority are ordered before incomplete
	///   tasks without one.
	/// - Two incomplete tasks with priorities are ordered by their
	///   priorities.
	/// - If none of the above apply, tasks are ordered by their created
	///   dates. Two tasks with identical created dates are unordered.
	// TODO: Need unit test demonstrating this works as intended
	friend std::partial_ordering operator<=>(const task &l, const task &r);

	friend bool operator==(const task &l, const task &r) = default;

	/// @brief The task's priority
	std::optional<char> priority = std::nullopt;

	/// @brief When the task was completed, if it has been completed
	/// @details This is set to `nullopt` by default, indicating the
	/// task is not yet complete.
	/// This time is stored as UTC.
	std::optional<std::chrono::sys_seconds> completed = std::nullopt;

	/// @brief When the task was created
	/// @details This is set to the current time by default.
	/// This time is stored as UTC.
	std::chrono::sys_seconds created = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());

	/// @brief When the task is due, if it has a due date
	/// @details This is set to `nullopt` by default, indicating the
	/// task has no due date.
	/// This time is stored as UTC.
	std::optional<std::chrono::sys_seconds> due = std::nullopt;

	/// @brief The task's description
	std::string name;
};

template<>
struct std::formatter<task> {
	static constexpr auto parse(std::format_parse_context& ctx) {
		// TODO: Display in local time with time zone offset by default
		// rather than UTC, but allow a formatter to enable UTC (which
		// is probably better for serialization/deserialization)
		return ctx.begin();
	}

	/// @brief Formats a @ref task
	static auto format(const task& t, std::format_context& ctx)  {
		const auto completed_date = t.completed.has_value()
			? std::format("{:%FT%TZ} ", t.completed.value())
			: "";
		const auto priority = t.priority
			? std::format("({}) ", *t.priority)
			: "";
		const auto due_date = t.due
			? std::format("due:{:%FT%TZ} ", *t.due)
			: "";
		return std::format_to(ctx.out(), "{}{}{}{:%FT%TZ} {}{}",
			t.completed ? "x " : "",
			priority,
			completed_date,
			t.created,
			due_date,
			t.name
		);
	}
};
