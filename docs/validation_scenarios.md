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



## GUI first test

---

### Scenario 1: task creation

* **Objective:** Verify that a user can create a new a task .
* **Results:**



&#x20;![New task image](https://github.com/user-attachments/assets/ea2b2c0b-32d7-4d67-b3c1-266c5680aeec)



* Status: PASS







### Scenario 2: Displaying task

* **Objective:** Verify that stored tasks and their completion statuses are properly display 
* **Results:**


&#x20;  ![all list image](https://github.com/user-attachments/assets/4ca5db84-e38c-4555-b1d4-acf05da76dfb)



* Status: PASS





### Scenario 3: Editing Existing Tasks

* **Objective:** verify that input validation handles missing parameters correctly and updates existing task descriptions upon receiving complete arguments.

* **Results:**


&#x20;  ![test image 3.1](https://github.com/user-attachments/assets/e504fcac-c9bc-444e-a624-871eec3a0fc9)

&#x20;  ![test image 3.2](https://github.com/user-attachments/assets/d7d8a13e-138f-49c9-b131-328f3b3fe41a)



* Status: PASS






### Scenario 4: task completion & reopening

* **Objective:** Verify that task status toggles between complete [x] and incomplete [ ].
* **Results:**

&#x20;  ![In all](https://github.com/user-attachments/assets/540259cd-b1b1-4425-ad68-497bfc903cf9)

&#x20;  ![Incomplete](https://github.com/user-attachments/assets/1b4de41c-3cfe-44ef-a730-e772335a31e9)

&#x20;  ![Complet](https://github.com/user-attachments/assets/23bd1b10-786c-4ff9-ae98-f7b2bc036587)

* Status: PASS



### Scenario 5: task deletion

* **Objective:** Safely remove a task from the list and persist changes.
* **Results:**


&#x20; ![test image 4](https://github.com/user-attachments/assets/a47e00bf-d2c3-4213-97d6-4c8e24c28872)

&#x20; ![test image 4](https://github.com/user-attachments/assets/bf2b2c47-701a-43a0-b7b2-6bb4f98c3794)



* Status: PASS


### Scenario 6: urgence task

* **Objective:** see if the tasks will be placed according to their value in [].
* **Results:**

In this image, you can see that the first task is of type [A].
&#x20; ![test image 5](https://github.com/user-attachments/assets/8430b80d-2a59-4750-96f9-27c48beb39e2)


after modification at level[Z] and its position changed.
&#x20; ![test image 5](https://github.com/user-attachments/assets/c58590a8-f250-46f7-8d82-be189d326e37)



* Status: PASS

---



&#x20;








