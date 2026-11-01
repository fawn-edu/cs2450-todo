# Validation scenarios \& Test report



---



## Executive Summary

This document records the manual test execution and verification of the refactored \*\*Student Task Manager\*\* application. Although the target graphical user interface (GUI) designed with Dear ImGUI is scheduled for a future milestone release, all core task management capabilities, Model-View-Controller (MVC) data flows, and architectural SOLID principles have been fully validated using the CLI interface.



---



## System Architecture \& Verification Scope



1. **MVC Implementation:**

&#x20;  - **Model:** Represents individual task properties (name, completion status, priority).

&#x20;  - **Controller ('task\_controller'):** Handles core logic, argument parsing, state modification, and persistent storage via 'todo.txt'.

&#x20;  - **View ('task_view'):** Formats and outputs task states and status messages to the terminal.



2. **SOLID principales verified:**

&#x20;  - **Single Responsibility Principle:** Display logic ('task_view') is decoupled from data mutation and file I/0 ('task_controller').

&#x20;  - **Open/Closed Principle:** New task commands can be add to the controller without altering the structural definition of a task.



---



## Executed Test Scenarios



### Scenario 1: task creation

* **Objective:** Verify that a user can create a new a task with a specified description.
* **Command executed:**



&#x20;![test image 1](https://github.com/user-attachments/assets/a2d2cce7-bfed-42da-a7ec-69eae1d632a1)



* Status: PASS







### Scenario 2: Displaying task

* **Objective:** Verify that stored tasks and their completion statuses are properly display 
* **Command executed:**

&#x20;  ./Debug/todo.exe list

* **Expected Output:**

&#x20;  ![test image 2](https://github.com/user-attachments/assets/23026840-42f2-43ef-8881-eca8777bb216)



* Status: PASS





### Scenario 3: Editing Existing Tasks

* **Objective:** verify that input validation handles missing parameters correctly and updates existing task descriptions upon receiving complete arguments.
* **first try Command executed:**

&#x20;   ./Debug/todo.exe edit "1/2 homework c++'

* **first Expected Output:**

&#x20; ![test image 3.1](https://github.com/user-attachments/assets/0a6cbde8-1254-4d3c-afef-e32f567b84c2)



* **Command executed:**

&#x20;   ./Debug/todo.exe edit "Homework c++" "1/2 homework c++"

* **Expected Output:**

&#x20;  ![test image 3.2](https://github.com/user-attachments/assets/c85c208d-1fb3-411e-a4ba-9f0a013e0471)



* Status: PASS






### Scenario 4: task completion & reopening

* **Objective:** Verify that task status toggles between complete [x] and incomplete [ ].
* **Command executed:**

&#x20;  ./Debug/todo.exe complete "1/2 homework c++"

&#x20;  ./Debug/todo.exe reopen "1/2 homework c++"



* **Expected Output:** Task status toggles accordingly and updates local file



* Status: PASS



### Scenario 5: task deletion

* **Objective:** Safely remove a task from the list and persist changes.
* **Command executed:**

&#x20; ./Debug/todo.exe delete "1/2 homework c++"



* **Expected Output:**

&#x20; ![test image 4](https://github.com/user-attachments/assets/b8fff395-b345-4066-bcac-adb2b85348a3)



* Status: PASS

&#x20;



---



### Next test 

for the next test is for GUI


