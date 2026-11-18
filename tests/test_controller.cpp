#include <cassert>
#include <iostream>
#include <vector>
#include <optional>
#include "task.hpp"
#include "task_controller.hpp"

void run_controller_tests() {
	std::cout << "\n--- Running Integration Tests: task_controller ---\n";

	//11. Adding a valid task
	{
		std::vector<task> tasks;
		task_controller controller(tasks);

		controller.add_task("First task", 'B', std::nullopt);
		assert(controller.get_tasks().size() == 1);
		assert(controller.get_tasks()[0].name == "First task");
		assert(controller.get_tasks()[0].priority == 'B');
		std::cout << "[PASS] test_controller_add_task\n";
	}

	//12. Adding a task with empty name defaults to "New task"
	{
		std::vector<task> tasks;
		task_controller controller(tasks);

		controller.add_task("", std::nullopt, std::nullopt);
		assert(controller.get_tasks().size() == 1);
		assert(controller.get_tasks()[0].name == "New task");
		std::cout << "[PASS] test_controller_add_empty_name_default\n";
	}

	//13. Editing an existing task
	{
		std::vector<task> tasks;
		task_controller controller(tasks);

		controller.add_task("Old name", 'C', std::nullopt);
		controller.edit_task(0, "Updated name", 'A', std::nullopt);

		assert(controller.get_tasks()[0].name == "Updated name");
		assert(controller.get_tasks()[0].priority == 'A');
		std::cout << "[PASS] test_controller_edit_task\n";
	}

	//14. Editing out-of-bounds index safely does nothing
	{
		std::vector<task> tasks;
		task_controller controller(tasks);

		controller.add_task("Single task", 'A', std::nullopt);
		controller.edit_task(99, "Should not crash", 'B', std::nullopt);

		assert(controller.get_tasks().size() == 1);
		assert(controller.get_tasks()[0].name == "Single task");
		std::cout << "[PASS] test_controller_edit_out_of_bounds\n";
	}

	//15. Deleting an existing task
	{
		std::vector<task> tasks;
		task_controller controller(tasks);

		controller.add_task("Task to delete", std::nullopt, std::nullopt);
		assert(controller.get_tasks().size() == 1);

		controller.delete_task(0);
		assert(controller.get_tasks().empty());
		std::cout << "[PASS] test_controller_delete_task\n";
	}

	//16. Deleting out-of-bounds index safely does nothing
	{
		std::vector<task> tasks;
		task_controller controller(tasks);

		controller.add_task("Keep me", std::nullopt, std::nullopt);
		controller.delete_task(42);
		assert(controller.get_tasks().size() == 1);
		std::cout << "[PASS] test_controller_delete_out_of_bounds\n";
	}

	//17. Toggling task completion status
	{
		std::vector<task> tasks;
		task_controller controller(tasks);

		controller.add_task("Toggle completion", std::nullopt, std::nullopt);
		assert(!controller.get_tasks()[0].completed.has_value());

		controller.set_completed(0, true);
		assert(controller.get_tasks()[0].completed.has_value());

		controller.set_completed(0, false);
		assert(!controller.get_tasks()[0].completed.has_value());

		std::cout << "[PASS] test_controller_set_completed\n";
	}
}