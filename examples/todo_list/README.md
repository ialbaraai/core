# Todo List - core example

A simple interactive command-line task manager demonstarting the `core` library's OOP syste, `string_t`, and `vector_t`.

## Folder structure

```
todo_list/
    include/
        tasks.h
    src/
        tasks.c
    main.c
    CMakeLists.txt
```

## Building

### 1. Build core first

From the repository root:

```bash
cmake -S . -B build
cmake --build build
```

### 2. Build the example

From `examples/todo_list` director:

```bash
cmake -S . -B build
cmake --build build
```

### 3. Run

```bash
./build/todo_app
```

## Usage

```
---------- Tasks Manager ----------
What would you like to do?
    1. List tasks
    2. Add task
    3. Adjust task
    4. Quit
Choose an option: 
```

 - **1** - list all current tasks
 - **2** - add a new task
 - **3** - find a task by name and replace it
 - **4** - quit