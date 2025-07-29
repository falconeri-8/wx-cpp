# DataManager - C++ File I/O Utility Library

A lightweight, template-based C++ library for simplified file operations with support for multiple data types and batch processing.
<br><br>
## DataManager.h

Template-based file I/O utility library with the following functions:

`Write<typename... Args>(filename, vars...)`

Appends multiple variables to a file, space-separated on a new line.
```cpp
Write("data.txt", name, age, salary);
```

`Read<typename... Args>(filename, vars...)`

Reads the first line from a file into the provided variables.
```cpp
Read("data.txt", name, age, salary);
```

`Fetch<typename T>(filename, vector<T>&)`

Loads all data from a file into a vector.
```cpp
vector<string> data;
Fetch("data.txt", data);
```

`Log(filename)`

Displays the complete contents of a file to console output.
```cpp
Log("data.txt");
```
<br>

### Example (main.cpp)

- Login system with hardcoded credentials (admin/password)
- Profile creation form with data persistence
- Uses DataManager's `Write()` function to save profile data

#### Requirements

- C++17 or later
- wxWidgets 3.0+
