# Command Pattern Implementation (Milestone 4)

## Purpose

The task manager uses the Command design pattern to encapsulate GUI-triggered task operations behind a common `TaskCommand::execute(task_controller&)` interface. This keeps task mutation in the existing controller and gives the GUI a consistent way to invoke actions without duplicating business logic.

## Design

- `TaskCommand` is the abstract command interface.
- `SaveTaskCommand` combines creation and editing. An absent task index creates a task; a supplied index edits an existing task. Both paths share `TaskInput` (name, priority, due date).
- `DeleteTaskCommand` deletes the selected task.
- `CompleteTaskCommand` sets the selected task's completion state, supporting both completion and reopening.
- `task_controller` remains the receiver and retains the existing add/edit/delete/complete operations.
- `task_view` acts as the invoker, constructing a command after GUI validation and executing it.

```text
FLTK View (invoker)
  | creates and executes
  v
TaskCommand::execute(task_controller&)
  ^-- SaveTaskCommand (create/edit)
  ^-- DeleteTaskCommand
  ^-- CompleteTaskCommand
  |
  v
task_controller (receiver) -> task model
```

## Why this pattern?

The existing GUI already has a shared create/edit dialog. A single save command makes that common workflow explicit, while all task operations can be executed through the same interface. This also makes each action easier to isolate in future tests. The implementation does **not** add undo, redo, a history stack, or state snapshots.

## Tradeoffs and alternatives

Direct controller calls would be simpler for a small application, and a helper function could combine create/edit with fewer classes. The Command pattern adds small classes but provides a consistent action boundary for a growing GUI. We intentionally use stack-allocated commands rather than introducing a command queue or dispatcher that the application does not need.

## MVC / SOLID

The view collects and validates input, commands encapsulate requests, the controller performs mutations, and the model stores task data. The view can invoke operations through the `TaskCommand` interface (dependency inversion), and new commands can be introduced without modifying that interface (open/closed principle).

## Files changed

- `src/task_command.hpp` — command interface and concrete commands
- `src/task_view.cpp` — execute commands in save, delete, and completion callbacks

## Manual test checklist

1. Create a task with name, priority, and due date.
2. Edit an existing task and confirm the updated values appear.
3. Complete a task and reopen it.
4. Delete a task after confirming; cancel deletion and verify the task remains.
5. Verify invalid task name, priority, and due date still show validation messages.
6. Restart the application and verify tasks persist in `todo.txt`.

**Status:** Implemented in source; compilation and GUI behavior require verification in the team's C++23/FLTK build environment.
