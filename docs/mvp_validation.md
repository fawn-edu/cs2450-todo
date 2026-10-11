## Issue #2 - View Task List

### Test 1 - Empty Task List

Command:
`.\main.exe list`

Result:
`No tasks found. Create a task with the 'create' command.`

Status: PASS

### Test 2 - Task List with Tasks

For testing, I temporarily added two tasks:
- Homework (Incomplete)
- CS 2450 Assignment (Completed)

Command:  
`.\main.exe list`

Result:
```
[ ] Homework
[x] CS 2450 Assignment
```

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
```

## Issue #4 - Complete and Reopen Tasks

### Test Setup

Since task persistence is not implemented yet, I temporarily added these tasks for testing:

- Homework
- CS2450 Homework
- Completed Assignment (Completed)

The temporary tasks were removed after testing.

### Test 1 - Ambiguous Task Name

Command:
`.\main.exe complete Homework`

Expected Result:
The program should show an error because more than one task matches "Homework".

Actual Result:
`Task name is ambiguous: Homework`

Status: PASS

### Test 2 - Complete a Task

Command:
`.\main.exe complete CS2450`

Expected Result:
CS2450 Homework should be marked as completed.

Actual Result:
CS2450 Homework was marked as completed with an `x` and completion time.

Status: PASS

### Test 3 - Reopen a Task

Command:
`.\main.exe reopen "Completed Assignment"`

Expected Result:
Completed Assignment should be changed back to incomplete.

Actual Result:
Completed Assignment was changed back to incomplete.

Status: PASS

### Test 4 - Task Not Found

Command:
`.\main.exe complete Biology`

Expected Result:
The program should show an error because there is no matching task.

Actual Result:
`Task not found: Biology`

Status: PASS
