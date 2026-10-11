#set document(
	title: [CS2450 Task Manager, v1.0],
	author: (
			"Fawn Sannar",
			"Trey Bailey",
			"Zaq Holdsworth",
	)
)
#set page(numbering: "1")

#show heading: set block(below: 1em)
#show link: set text(fill: blue)

#align(center)[
	#title()
	#context document.author.join(", ")

	#link("https://github.com/fawn-edu/cs2450-taskmanager/releases/tag/v1.0")[Release v1.0], #link("https://github.com/users/fawn-edu/projects/1")[Project Board]

	#datetime.today().display()
]

#figure(
	image("meeting_minutes/Screenshot 2026-10-03 at 16-22-30 Chat Alycia Mortensen Microsoft Teams.png", width: 100%),
	caption: [Professor Mortensen clarifies v1.0 release expectations.],
)

= Changelog since v0.1

- `63ed6f7`, `f18884b`, `a0acdd5`, `b4398ce`: Add Zaq Holdsworth as a team member, including team roles, new user stories, and participation in week 1 meetings that occurred after v0.1 was released.
- `5179346`: Revise communication plan to include our asynchronous text chat communication.
- `95f9fea`: Revise project timeline to include week 2 & the planned release of v1.0.
- `e08f9ad`: Consolidate Zaq's non-duplicate user stories into the project backlog.
- `93f8484`: Add meeting minutes for week 2.
- `b515ccb`: Remove unnecessary nonsense from project README.

= Team Formation

Our team has been marked for dissolution after Milestone 1 due to team members dropping the class, so this document might not have exactly what was intended. Please grade gently with this in mind.

== Roles

- Fawn Sannar: meeting recorder, developer, tester
- Trey Bailey: scrum master, developer, tester
- Zaq Holdsworth: designer, developer, and UX research

== Communication plan

Our original communication plan was to meet over Microsoft Teams on Tuesdays from 11:30–13:00. However, our team is unable to actually meet, so we will asynchronously communicate over Microsoft Teams text chat as our individual availability permits. Our communication plan is to adhere to whatever our future teams' communication plan is as we will be soon reassigned.

= Product Vision

Task Manager is a browser-based to-do list for users to organize tasks and everyday responsibilities. Its goal is to help users capture what needs to be done, identify urgent work, and track their progress in one place.

The initial version will let users create, view, edit, delete, complete, and reopen tasks. Optional due dates and High, Medium, or Low priorities will help users decide what to work on next, while sorting and filtering will help them focus on relevant tasks. Tasks will be saved in the same browser so users can return to their list after reloading the page.

Our timeline is as such:

- Week 1: Deliver Milestone 1, which involves setting up the repository, organizing user stories, and assigning roles
- Week 2: Deliver v1.0, which will clarify and finalize project documentation
- Week 3: Abandon it all as we are reassigned to our new teams for future milestones, adopting their planned timelines

Risks mainly include the grader finding the honesty of this timeline unamusing.

= Initial Backlog

- *Create Tasks Functionality:* As a busy user, I want to capture a task so I can remember work I need to do.
- *View Task List:* As a user, I want to see my tasks together so I can plan my work.
- *Edit Tasks:* As a user, I want to correct task details so my list stays accurate.
- *Complete and Reopen Tasks:* As a user, I want to mark work complete and reopen it so progress stays accurate.
- *Delete Tasks Safely:* As a user, I want to remove unwanted tasks so my list stays useful.
- *Set Due Dates and Show Overdue Tasks:* As a user, I want to set deadlines so I can identify urgent work.
- *Set Task Priorities:* As a user, I want to rank task importance so I can choose what to do next.
- *Save Tasks Between Visits:* As a user, I want my list to survive a browser reload so I do not lose my work.
- *Filter Tasks by Status and Priority:* As a user, I want to narrow my list so I can focus on relevant work.
- *Sort Tasks:* As a user, I want tasks ordered by deadline or importance so I can plan efficiently.
- *Responsive UI:* As a user, I want to be able to use the web application through either a desktop browser or a mobile browser, with comparable user experience.
- *Set Task Tags:* As a user, I want to add arbitrary tags to tasks so I can categorize them.
- *Filter Tasks by Tags:* As a user, I want to filter tags so I can focus on one category at a time, or so I can exclude a category, so that I can focus on relevant work.
- *Set Task Description:* As a user, I want to add extended descriptions to tasks using markdown formatting so that I can organize my thoughts related to any single task.
- *Search Tasks:* As a user, I want to search my tasks, taking into account task title, priority, tag, completion status, and task description (e.g. `status:complete -priority:low (tag:grocery | tag:shopping)` retrieves all completed tasks any priority except low with either the "grocery" or "shopping" tags).
- *Recurring Tasks:* As a shopper, I want some tasks to automatically reappear after a specified amount of time.
- *Set Task Duration:* As a student, I want tasks to have a attribute "time estimate" to automatically push tasks up when they take longer to complete.
- *Set Task Dependencies:* As a user, I want to link tasks together with relationships like after/before.

= Meeting Minutes

== Week 1

#figure(
	image("meeting_minutes/week1a.png", width: 100%),
	caption: [Week 1, part A],
)

#figure(
	image("meeting_minutes/week1b.png", width: 100%),
	caption: [Week 1, part B],
)

#figure(
	image("meeting_minutes/week1c.png", width: 100%),
	caption: [Week 1, part C],
)

= Week 2

#figure(
	image("meeting_minutes/week2a.png", width: 100%),
	caption: [Week 2, part A],
)
