# Contact Book - core example

A simple interactive command-line contact book manager demonstrating `core` library's OOP system, `string_t`, and `map_t`

## Folder structure

```
contact_book/
    include/
        contact_book.h
    src/
        contact_book.c
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

From `examples/contact_book` directory:

```bash
cmake -S . -B build
cmake --build build
```

### 3. Run

```bash
./build/contact_book_app
```

## Usage

```
---------- Contact Book ----------
What would you like to do?
    1. List contacts
    2. Add contact
    3. Adjust contact
    4. Quit
Choose an option:
```

 - **1** - list all current contacts with their numbers
 - **2** - add a new contact, with duplicate detection and override/rename/cancel prompt
 - **3** - find a contact by name and update their number
 - **4** - quit

## Notes

 - Contacts are case-insensitive - `Name` and `name` are treated as the same contact

---

## License

This project is licensed under the MIT License. See [LICENSE](./LICENSE) for details.

---

## Author

**albaraa** — [@ialbaraai](https://github.com/ialbaraai)