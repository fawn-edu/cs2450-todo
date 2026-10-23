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

	/// @brief Edits a task with values parsed from a `string_view`
	/// @details This function only parses information that might be
	/// changed during editing, such as task priority and name.
	/// @throws std::invalid_argument if @p serialized contains a
	/// line breaks or a malformed priority
	task &edit(std::string_view serialized);

	/// @brief Orders two tasks based on contents
	/// @details Orders tasks using the following criteria:
	/// - An incomplete task is ordered before an incomplete task.
	/// - Two complete tasks are ordered by their completed dates.
	/// - A task with no priority is ordered after a task with one.
	/// - Two tasks with priorities are ordered by their priorities.
	/// - If none of the above apply, tasks are ordered by their created
	///   dates.
	/// - If none of the above apply and tasks have identical created
	///   dates, they are unordered.
	// TODO: Need test demonstrating this works as intended
	friend std::partial_ordering operator<=>(const task &l, const task &r);

	friend bool operator==(const task &l, const task &r) = default;

	/// @brief The task's description
	std::string name;

	/// @brief When the task was created
	/// @details This is set to the current time by default.
	std::chrono::sys_seconds created = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());

	/// @brief When the task was completed, if it has been completed
	/// @details This is set to `nullopt` by default, indicating the
	/// task is not yet complete.
	std::optional<std::chrono::sys_seconds> completed = std::nullopt;

	/// @brief The task's priority
	std::optional<char> priority = std::nullopt;
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
		const auto *const is_completed = t.completed.has_value() ? "x " : "";
		const auto completed_date = t.completed.has_value()
			? std::format("{:%FT%T%z} ", t.completed.value())
			: "";
		const auto priority = t.priority.has_value()
			? std::format("({}) ", t.priority.value())
			: "";
		return std::format_to(ctx.out(), "{}{}{}{:%FT%T%z} {}",
			is_completed,
			priority,
			completed_date,
			t.created,
			t.name
		);
	}
};
