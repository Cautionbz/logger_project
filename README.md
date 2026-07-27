# Logger Project

A C++ logging library with file and socket-based loggers, plus example application.

## Project Structure

```
logger_project/
├── logger/           # Core logging library
│   ├── include/logger/
│   │   ├── logger.h       # Base interfaces and types
│   │   ├── file_logger.h  # File-based logger
│   │   └── socket_logger.h # TCP socket logger
│   └── src/
│       ├── logger.cpp
│       ├── file_logger.cpp
│       └── socket_logger.cpp
├── app_file/         # File logger demo application
│   └── main.cpp
└── tests/            # CppUTest unit tests
```

## Building the Project

### Prerequisites
- CMake 3.14+
- C++17 compatible compiler (GCC, Clang, MSVC)
- CppUTest for unit testing
- On Windows: Winsock2 library (linked automatically)

### Build with CMake

```bash
# Configure
cmake -B build
# Build
cmake --build build
```

This creates:
- `build/logger.lib` / `liblogger.a` - Core library
- `build/app_file/logger_app` - File logger demo
- `build/tests/logger_tests` - Unit tests

## Running the File Logger Demo (app_file)

The file logger writes log messages to a local file.

### Usage
```bash
./logger_app <log_file> <default_severity>
```

### Parameters
- `log_file` - Path to output log file (created if doesn't exist)
- `default_severity` - Minimum severity to log: `low`, `normal`, or `high`


### Interactive Mode
After starting, enter messages in the format:
```
[severity] <message>
```

- Severity is optional (uses default if omitted)
- Type `exit` to quit


### Output Format
Log entries are written as:
```
[2026-07-27 14:30:45] [High] Disk failure on /dev/sda
[2026-07-27 14:30:47] [Normal] User login: john
[2026-07-27 14:30:49] [Low] Debug: cache hit ratio 0.95
```

## Running Unit Tests

```bash
# Build tests (included in normal build)
cmake --build build --target logger_tests

# Run tests
./build/tests/logger_tests
```

## Windows Notes

- The socket logger automatically initializes Winsock (WSAStartup/WSACleanup)
- Link with `ws2_32.lib` (handled by CMake)
- Use `logger_app.exe` and `logger_sender.exe` on Windows
