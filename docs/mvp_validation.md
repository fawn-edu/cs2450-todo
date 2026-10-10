## Issue #2 - View Task List

### Test 1 - Empty Task List

Command:
.\main.exe list

Result:
No tasks found. Create a task with the 'create' command.

Status: PASS


### Test 2 - Task List with Tasks

For testing, I temporarily added two tasks:
- Homework (Incomplete)
- CS 2450 Assignment (Completed)

Command:
.\main.exe list

Result:
[] Homework
[x] CS 2450 Assignment

Status: PASS

The temporary tasks were removed after testing.

```cpp
tasks.emplace_back(task{.name = "Homework"});

tasks.emplace_back(task{
    .name = "CS 2450 Assignment",
    .completed = std::chrono::floor<std::chrono::seconds>(
        std::chrono::system_clock::now()
    )
});