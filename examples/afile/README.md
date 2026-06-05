# afile - core example

A simple interactive command-line file editor demonstrating the `core` library's OOP system, `file_t`, `string_t` and `vector_t`

## Folder structure

```
afile/
    include/
        bash.h
        write.h
        terminal.h
    src/
        bash.c
        write.c
        terminal.c
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

From `examples/afile` directory:

```bash
cmake -S . -B build
cmake --build build
```

### 3. Run

```bash
./build/afile
```

## Usage

```
$ file (file name)
$ clear
$ exit
$ quit
```

 - **`file (file name)`** - open or create a file and enter the editor
 - **`clear`** - clear the terminal (compatible with Windows and Unix)
 - **`exit`/`quit`** - quit afile

## Editor

Once inside a file, type line by line and press `Enter` to move to the next line. Press `Ctrl+O` to save and return to the prompt

```
> Hello, world!
> This is line 2
> ^O
File saved to "notes.txt"
$
```

---

## License

This project is licensed under the MIT License.

---

## Author

**albaraa** — [@ialbaraai](https://github.com/ialbaraai)
