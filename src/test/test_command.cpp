#include <cassert>
#include <iostream>
#include <vector>
#include "../task.hpp"
#include "../task_controller.hpp"
#include "../task_command.hpp"

int main() {
	std::cout << "\n--- Running Integration Tests: task_command ---\n";

	//18. SaveTaskCommand creates a new task when no index is provided
	{
		std::vector<task> tasks;
		task_controller controller(tasks);

		TaskInput input{ .name = "Created via command", .priority = 'A', .due = std::nullopt };
		SaveTaskCommand cmd(input, std::nullopt);
		cmd.execute(controller);

		assert(controller.get_tasks().size() == 1);
		assert(controller.get_tasks()[0].name == "Created via command");
		assert(controller.get_tasks()[0].priority == 'A');

		std::cout << "[PASS] test_save_command_create\n";
	}

	//19. SaveTaskCommand edits an existing task when index is provided
	{
		std::vector<task> tasks;
		task_controller controller(tasks);
		controller.add_task("Initial task", 'C', std::nullopt);

		TaskInput updated{ .name = "Edited via command", .priority = 'B', .due = std::nullopt };
		SaveTaskCommand edit_cmd(updated, 0);
		edit_cmd.execute(controller);

		assert(controller.get_tasks().size() == 1);
		assert(controller.get_tasks()[0].name == "Edited via command");
		assert(controller.get_tasks()[0].priority == 'B');

		std::cout << "[PASS] test_save_command_edit\n";
	}

	//20. CompleteTaskCommand marks task complete and reopens it
	{
		std::vector<task> tasks;
		task_controller controller(tasks);
		controller.add_task("Command completion", std::nullopt, std::nullopt);

		CompleteTaskCommand complete_cmd(0, true);
		complete_cmd.execute(controller);
		assert(controller.get_tasks()[0].completed.has_value());

		CompleteTaskCommand reopen_cmd(0, false);
		reopen_cmd.execute(controller);
		assert(!controller.get_tasks()[0].completed.has_value());

		std::cout << "[PASS] test_complete_command\n";

	}

	//21. DeeleteTaskCommand removes the specified task
	{
		std::vector<task> tasks;
		task_controller controller(tasks);
		controller.add_task("Command delete task", std::nullopt, std::nullopt);

		DeleteTaskCommand delete_cmd(0);
		delete_cmd.execute(controller);
		assert(controller.get_tasks().empty());
		std::cout << "[PASS] test_delete_command\n";
	}
}
