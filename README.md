# C Task Scheduler

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C Standard](https://img.shields.io/badge/C-C99-blue.svg)](https://en.wikipedia.org/wiki/C99)

A robust, highly modular asynchronous task scheduler implemented in C. This project features a clean layered data structure architecture, built from the ground up with custom implementations of core low-level containers, task encapsulation units, and unique identifiers.

---

## 🏗️ Layered Architecture & Dependencies

The scheduler relies on a strict composition of abstract data types and supporting modules, where each layer builds upon the clean interface of the previous one:

```text
  [ Task Scheduler ]
         │
         ├──► [ Priority Queue ] ───► [ Sorted List ] ───► [ Doubly Linked List ]
         │
         ├──► [ task.h / task.c ]   (Encapsulates individual task execution units and callbacks)
         └──► [ uid.h / uid.c ]     (Provides composite unique identifiers for task tracking)
```

### Module Breakdown
*   **Doubly Linked List (`dlist.c` / `dlist.h`)**: The foundational low-level container providing $O(1)$ insertions and deletions, implemented with dummy head/tail nodes for edge-case safety.
*   **Sorted List (`sorted_list.c` / `sorted_list.h`)**: Built on top of the doubly linked list, maintaining elements in sorted order according to a user-defined comparison function.
*   **Priority Queue (`pqueue.c` / `pqueue.h`)**: Built on top of the sorted list, exposing an enqueue/dequeue interface optimized for priority-based task ordering.
*   **Task Module (`task.c` / `task.h`)**: Encapsulates task parameters, callback function pointers, and execution timestamps.
*   **UID Module (`uid.c` / `uid.h`)**: Implements a composite Unique Identifier system combining timestamps, process IDs, and counters to guarantee global uniqueness across tasks.
*   **Scheduler (`scheduler.c` / `scheduler.h`)**: The core scheduling engine that ties everything together, utilizing the priority queue, tasks, and UIDs to manage asynchronous, timed execution loops.

---

## ✨ Key Features

*   **Strict Encapsulation & Opaque Pointers**: Data hiding across all layers to minimize coupling and enforce clean modularity.
*   **Composable Data Structures**: Reusable underlying containers designed with generic APIs and callback-driven design.
*   **Dynamic Task Management**: Supports adding tasks, cancelling pending tasks by UID, clearing the scheduler, and running continuous or timed execution loops.
*   **Robust Memory Safety**: Careful allocation and cleanup semantics throughout the stack to prevent leaks and handle edge cases safely.

---

## 📁 Project Structure

```text
├── include/       /* Public header files (apis) (scheduler.h, pqueue.h, sorted_list.h, dlist.h, task.h, uid.h) */
├── src/           /* Source implementation files */
├── test/          /* Test suites */
├── Makefile       /* Build configuration */
└── README.md
```

---

## 🚀 Getting Started

### Prerequisites

*   A C compiler (e.g., `gcc`)
*   `make`

### Building the Project

To compile the project and test binaries, run:

```bash
make
```

### Running Tests

Execute the test suites to verify functionality across all layers:
Tests are currently complicated prints to terminal and needs a rework.
```bash
make run
```

---

## 📝 Usage Example

```c
#include <stdio.h>
#include <time.h>
#include "scheduler.h"

int sample_task(void *param) {
    printf("Executing task with param: %s\n", (char *)param);
    return 0; /* Return 0 to stop repeating, or a positive value to reschedule interval */
}

int main(void) {
    scheduler_t *sched = SchedCreate();
    
    /* Schedule a task to run after 2 seconds */
    uid_t task_uid = SchedAddTask(sched, sample_task, "Hello, World!", 2);

    /* Run the scheduler loop */
    SchedRun(sched);

    SchedDestroy(sched);
    return 0;
}
```

--
