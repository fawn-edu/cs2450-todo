#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include "../task.hpp"

int main() {
	std::cout << "Running task tests...\n";

	// 1. default task creation
	{
		task t{.name="Important Task"};
		assert(t.name == "Important Task");
		assert(!t.completed.has_value());
		assert(!t.priority.has_value());
		assert(!t.due.has_value());
		std::cout << "[PASS] test_task_creation\n";
	}

	// 2. Completed task 
	{
		task t{.name="Completed Task"};
		assert(!t.completed.has_value());
		t.complete();
		assert(t.completed.has_value());
		auto initial_completed = t.completed;
		t.complete();
		assert(t.completed == initial_completed);
		std::cout << "[PASS] test_completed_task_creation\n";
	}

	// 3. Parsing a valid basic task
	{
		std::string serialized = "2026-03-30T10:00:00Z Write milestone report";
		task t = task::parse(serialized);
		assert(t.name == "Write milestone report");
		assert(!t.completed.has_value());
		assert(!t.priority.has_value());
		assert(!t.due.has_value());
		std::cout << "[PASS] test_parse_basic_task\n";
	}

	// 4. Parsing a valid task with priority and due date
	{
		std::string serialized = "(A) 2026-03-30T10:00:00Z due:2026-04-01T23:59:59Z Pay utility bills";
		task t = task::parse(serialized);
		assert(!t.completed.has_value());
		assert(t.priority.has_value());
		assert(*t.priority == 'A');
		assert(t.due.has_value());
		assert(t.name == "Pay utility bills");
		std::cout << "[PASS] test_parse_task_with_all_fields\n";
	}

	// 5. Parsing a completed task
	{
		std::string serialized = "x (B) 2026-03-31T12:00:00Z 2026-03-30T10:00:00Z Completed Project";
		task t = task::parse(serialized);
		assert(t.completed.has_value());
		assert(t.priority.has_value());
		assert(*t.priority == 'B');
		assert(!t.due.has_value());
		assert(t.name == "Completed Project");
		std::cout << "[PASS] test_parse_completed_task\n";
	}

	// 6. Parsing invalid empty task throws exception
	{
		bool threw = false;
		try {
			task::parse(" ");
		}
		catch (const std::invalid_argument&) {
			threw = true;
		}
		assert(threw);
		std::cout << "[PASS] test_task_parsing_empty_throws\n";
	}

	// 7. Parsing invalid priority format throws exception
	{
		bool threw = false;
		try {
			task::parse("(1) 2026-03-30T10:00:00Z Invalid priority");
		}
		catch (const std::invalid_argument&) {
			threw = true;
		}
		assert(threw);
		std::cout << "[PASS] test_task_parsing_priority_throws\n";
	}

	// 8. Parsing illegal line breaks throws exception
	{
		bool threw = false;
		try {
			task::parse("2026-03-30T10:00:00Z Task with\r\nnewline");
		}
		catch (const std::invalid_argument&) {
			threw = true;
		}
		assert(threw);
		std::cout << "[PASS] test_task_parsing_crlf_throws\n";
	}

	// 9. Ordering: incomplete task comes before completed task
	{
		task incomplete_task{.name = "Incomplete"};
		task complete_task{.name = "Completed"};
		complete_task.complete();
		assert((incomplete_task <=> complete_task) == std::partial_ordering::less);
		std::cout << "[PASS] test_task_ordering_completion\n";
	}

	// 10. Ordering: higher priority (A) comes before lower priority (B)
	{
		task high_prio{.priority = 'A', .name = "High"};
		task low_prio{.priority = 'B', .name = "Low"};
		assert((high_prio <=> low_prio) == std::partial_ordering::less);
		std::cout << "[PASS] test_task_ordering_priority\n";
	}

	// 11. Ordering: newer created dates come before older created dates
	{
		task old_task;
		task new_task;
		new_task.created += std::chrono::seconds{1};
		assert((old_task <=> new_task) == std::partial_ordering::less);
		std::cout << "[PASS] test_task_ordering_created\n";
	}

	// 12. Ordering: if none of the above apply, tasks are unordered
	{
		task a;
		task b{a};
		assert((a <=> b) == std::partial_ordering::unordered);
		std::cout << "[PASS] test_task_ordering_unordered\n";
	}
}
