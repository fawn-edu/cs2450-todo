#pragma once

#include <chrono>
#include <string>
#include <optional>

/// @brief An individual todo-list task
struct task {
	/// @brief Parses a task from a `string_view`
	/// @throws std::invalid_argument if @p serialized is not a valid
	/// text representation of a @ref task
	/// @see https://github.com/todotxt/todo.txt#todotxt-format-rules
	static task parse(std::string_view serialized);

	/// @brief The task's description
	std::string name;

	/// @brief When the task was created
	/// @details This is set to the current time by default.
	std::chrono::sys_seconds created = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());

	/// @brief When the task was completed, if it has been completed
	/// @details This is set to `nullopt` by default, indicating the
	/// task is not yet complete.
	std::optional<std::chrono::sys_seconds> completed = std::nullopt;
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
		const auto completed_date = t.completed
			? std::format("x {:%FT%TZ%Z} ", t.completed.value())
			: "";
		return std::format_to(ctx.out(), "{}{:%FT%TZ%Z} {}",
			completed_date,
			t.created,
			t.name
		);
	}
};
