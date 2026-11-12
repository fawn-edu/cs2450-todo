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



&#x20;![New task image](https://github.com/user-attachments/assets/ae7a3747-abd8-44fc-9b14-41b9d51b53a0)



* Status: PASS







### Scenario 2: Displaying task

* **Objective:** Verify that stored tasks and their completion statuses are properly display 
* **Results:**


&#x20;  ![all list image](https://github.com/user-attachments/assets/de6a92b7-24c2-4ab5-8015-8770d6bf8995)



* Status: PASS





### Scenario 3: Editing Existing Tasks

* **Objective:** verify that input validation handles missing parameters correctly and updates existing task descriptions upon receiving complete arguments.

* **Results:**


&#x20;  ![test image 3.1](https://github.com/user-attachments/assets/35a0a5cd-8d4a-4bad-8fe9-83fa7bf7ff48)

&#x20;  ![test image 3.2](https://github.com/user-attachments/assets/d3b2608f-74f4-4bfb-98d5-01d18f98282d)



* Status: PASS






### Scenario 4: task completion & reopening

* **Objective:** Verify that task status toggles between complete [x] and incomplete [ ].
* **Results:**

&#x20;  ![In all](https://github.com/user-attachments/assets/848a3e26-9833-4e48-a967-17e6c2613dd6)

&#x20;  ![Incomplete](https://github.com/user-attachments/assets/78bf67c8-0a1f-4329-9d69-d39e8137e508)

&#x20;  ![Complet](https://github.com/user-attachments/assets/5478faf2-03d5-41a7-b924-ac65c655256d)

* Status: PASS



### Scenario 5: task deletion

* **Objective:** Safely remove a task from the list and persist changes.
* **Results:**


&#x20; ![test image 4](https://github.com/user-attachments/assets/40349b0c-29a0-4bc2-90c6-dc9e9f9e0b1c)

&#x20; ![test image 4](https://github.com/user-attachments/assets/d7ca747b-0702-43ce-b335-578c98f90aaf)



* Status: PASS


### Scenario 6: urgence task

* **Objective:** see if the tasks will be placed according to their value in [].
* **Results:**

In this image, you can see that the first task is of type [A].
&#x20; ![test image 5](https://github.com/user-attachments/assets/f6df0aaa-8497-4b0c-a316-61da4757d7ed)


after modification at level[Z] and its position changed.
&#x20; ![test image 5](https://github.com/user-attachments/assets/31710042-1da7-42e9-9cb1-ade8ae0b02a8)



* Status: PASS

---



&#x20;








