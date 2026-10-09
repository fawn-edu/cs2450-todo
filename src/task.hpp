#pragma once

#include <chrono>
#include <string>
#include <optional>

/// @brief An individual todo-list task
struct task {
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
	constexpr auto parse(std::format_parse_context& ctx) {
		// TODO: Display in local time with time zone offset by default
		// rather than UTC, but allow a formatter to enable UTC (which
		// is probably better for serialization/deserialization)
		return ctx.begin();
	}

	/// @brief Formats a @ref task
	auto format(const task& t, std::format_context& ctx) const {
		const auto completed_date = t.completed.has_value()
			? std::format("x {:%FT%TZ%Z} ", t.completed.value())
			: "";
		return std::format_to(ctx.out(), "{}{:%FT%TZ%Z} {}",
			completed_date,
			t.created,
			t.name
		);
	}
};
