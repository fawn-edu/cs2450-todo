#include <iostream>
void run_task_tests();
void run_controller_tests();
void run_command_tests();

int main() {
	std::cout << "=============================\n";
	std::cout << "CS 2450 TASK MANAGER - TEST SUITE (21)\n";
	std::cout << "=============================\n";

	run_task_tests();
	run_controller_tests();
	run_command_tests();

	std::cout << "=============================\n";
	std::cout << "All 21 tests passed successfully!\n";
	std::cout << "=============================\n";

	return 0;
}