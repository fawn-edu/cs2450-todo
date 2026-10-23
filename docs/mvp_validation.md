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


## CLI operation

```
$ make test
clang++ -std=c++23 -O2 -Wall -Wextra -pedantic -o todo.out main.cpp task.cpp
rm -f todo.txt
./todo.out create "(d) Test task 1"
./todo.out complete "1"
./todo.out reopen "1"
./todo.out create "(b) testt task  2"
./todo.out complete "2"
./todo.out create "(c) Test task 3"
./todo.out delete "3"
./todo.out edit "1" "(a)"
./todo.out edit "2" "Test task 2"
./todo.out list
[ ] (A) Test task 1
[x] (B) Test task 2
```

## Tasks are sorted correctly

- Completed tasks appear after incomplete tasks and are sorted in the order they were completed
- Incomplete tasks are sorted by priority, with no priority being ordered after all other priorities, then sorted by created date

```
$ make test-sort
clang++ -std=c++23 -O2 -Wall -Wextra -pedantic -o todo.out main.cpp task.cpp
rm -f todo.txt
./todo.out create "No priority, incomplete 1"
./todo.out create "No priority, complete 1"
./todo.out create "(A) A priority, incomplete 1"
./todo.out create "(A) A priority, complete 1"
./todo.out create "(B) B priority, incomplete 1"
./todo.out create "(B) B priority, complete 1"
sleep 1s
./todo.out create "No priority, incomplete 2"
./todo.out create "No priority, complete 2"
./todo.out create "(A) A priority, incomplete 2"
./todo.out create "(A) A priority, complete 2"
./todo.out create "(B) B priority, incomplete 2"
./todo.out create "(B) B priority, complete 2"
sleep 1s
./todo.out complete "No priority, complete 1"
./todo.out complete "A priority, complete 1"
./todo.out complete "B priority, complete 1"
sleep 1s
./todo.out complete "No priority, complete 2"
./todo.out complete "A priority, complete 2"
./todo.out complete "B priority, complete 2"
cat todo.txt
2026-10-23T01:29:26+0000 (A) A priority, incomplete 1
2026-10-23T01:29:27+0000 (A) A priority, incomplete 2
2026-10-23T01:29:26+0000 (B) B priority, incomplete 1
2026-10-23T01:29:27+0000 (B) B priority, incomplete 2
2026-10-23T01:29:25+0000 No priority, incomplete 1
2026-10-23T01:29:27+0000 No priority, incomplete 2
x 2026-10-23T01:29:28+0000 2026-10-23T01:29:26+0000 (A) A priority, complete 1
x 2026-10-23T01:29:28+0000 2026-10-23T01:29:26+0000 (B) B priority, complete 1
x 2026-10-23T01:29:28+0000 2026-10-23T01:29:26+0000 No priority, complete 1
x 2026-10-23T01:29:29+0000 2026-10-23T01:29:27+0000 (A) A priority, complete 2
x 2026-10-23T01:29:29+0000 2026-10-23T01:29:27+0000 (B) B priority, complete 2
x 2026-10-23T01:29:29+0000 2026-10-23T01:29:27+0000 No priority, complete 2
```
