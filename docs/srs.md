# Software Requirements Specification
## For Student To-Do List - MVP

**Version** 0.1  
**Class:** CS-2450-003
**Date:** 10-10-2026
**Team:** Fawn Sannar, Ethan Scott, Porter Sampson, Jimin Ko, Mckley Carmebi Leprince Florestal

## Revision History

| Name   | Date       | Reason For Changes | Version |
| ------ | ---------- | ------------------ | ------- |
| Porter | 10-10-2026 | Init - Milestone 2 | 0.1     |
|        |            |                    |         |

## 1. Introduction & Purpose

This Student Todo List is a simple application, currently CLI based, designed to help students organize their work. This initial release focuses on the basic functions of a todo list including: creating, viewing, editing, completing, re-opening, saving and deleting tasks.

This SRS documents the MVP's requirements and boundaries for **Milestone 2**. More features from the projects backlog/issue board will be added in later milestones. 


## 2. Overall Description

**System Context:** The current design is a command-line application written in C++. Tasks will be saved locally in a plain-text file following the [todo.txt format](https://github.com/todotxt/todo.txt). The program will load saved tasks so they remain available between runs.

**Users:** The primary users are students who need a straightforward way to keep track of assignments and other responsibilities. For this MVP, users are assumed to know basic terminal commands to execute the application. 

**MVP scope:** The five core task-management features listed below, plus persistent storage. The team has also discussed a command to demonstrate functionality using isolated sample data and tests for the implemented features.


| Issue No. | **User Story**                                                                    | **Backlog Issue**          |
| --------- | --------------------------------------------------------------------------------- | -------------------------- |
| 1         | As a busy user, I want to capture a task so I can remember work I need to do.     | Create Tasks Functionality |
| 2         | As a user, I want to see my tasks together so I can plan my work.                 | View Task List             |
| 4         | As a user, I want to mark work complete and reopen it so progress stays accurate. | Complete and Reopen Tasks  |
| 5         | As a user, I want to remove unwanted tasks so my list stays useful.               | Delete Tasks Safely        |
| 8         | As a user, I want to save tasks so my list persists after closing                 | Save Tasks Between Visits  |

## 4. Functional Requirements

| Related story | Requirement                                                                                          |
| ------------- | ---------------------------------------------------------------------------------------------------- |
| 1             | The application should allow users to create a task with a title.                                    |
| 2             | The application should display saved tasks and indicate whether each is complete or incomplete.      |
| 4             | The application should allow users to mark tasks complete and reopen completed tasks.                |
| 5             | The application should allow users to delete a selected task.                                        |
| 8             | The application shall save task changes to a plain-text file and reload them in a later program run. |

**Deliverables:** For **Milestone 2** the plan is to create a demonstration of the basic features working in the CLI.

## 5. Non-Functional Requirements
| Category    | Requirement                                                                                                               |
| ----------- | ------------------------------------------------------------------------------------------------------------------------- |
| Usability   | The command-line interface should inform the user when a task operation succeeds or fails.                                |
| Performance | The application should respond promptly to ordinary task operations. A measurable time target and test-file size are TBD. |
| Reliability | The application should indicate file read/write errors rather than silently claiming changes were saved.                  |
| Platform    | The MVP application will be implemented in C++ and run through a CLI.                                                     |
| Storage     | The application should use a local text file compatible with the agreed todo.txt conventions.                             |

## 6. Use Cases & UML Diagrams

**Note:** We've opted to go with procedural code. Since we do not have classes to model a class diagram we've instead included a conceptual model to convey the same system information as a class diagram. 

**Primary Actor:** User / Student

| Use case        | User interaction                            | Expected result                                        |
| --------------- | ------------------------------------------- | ------------------------------------------------------ |
| Create task     | User enters a new task title.               | The task is saved and appears in the list.             |
| View tasks      | User requests the task list.                | Saved tasks and their completion status are displayed. |
| Complete/reopen | User selects a task and changes its status. | The new status is saved.                               |
| Delete task     | User selects a task for deletion.           | The task is deleted.                                   |

#### Use Case Diagram
![[usecasediagram.png]]
#### Conceptual Model 

This is the task struct used in our MVP. We will store multiple tasks using `std::vector<task>` and serialize them to a text file. 
![[taskstruct.png]]

This is a model representing *main.cpp* and shows how the program will process commands from the CLI 
![[programbehavior.png]]

## 7. Assumptions & Constraints

#### Assumptions
- User understands basic CLI usage.
- Application can read/write to the local filesystem.
- Application can run offline.

#### Constraints
- MVP's interface uses CLI.
- Written in C++.
- Persistent task storage will use a file based on the todo.txt format.

**Document Evolution:** Additional sections may be re-added from the original template as future milestones require them.