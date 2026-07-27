// Main application entry point for the logger project.
// This application demonstrates the FileLogger functionality by accepting log messages from stdin
// and writing them to a log file with severity levels.

#include "../logger/include/logger/file_logger.h"
#include <iostream>
#include <thread>
#include <queue>
#include <condition_variable>
#include <sstream>
#include <atomic>

using namespace logger;

// Structure to hold a log message and its severity level for the worker thread queue
struct MessageItem {
    std::string text;
    Severity sev;
};

// Parses a severity string (low, normal, high) into a Severity enum value.
// Returns true via 'ok' parameter if parsing succeeds, false otherwise.
static Severity parseSeverity(const std::string& s, bool& ok) {
    ok = true;
    if (s == "low") return Severity::Low;
    if (s == "normal") return Severity::Normal;
    if (s == "high") return Severity::High;
    ok = false;
    return Severity::Normal;
}

// Main entry point: reads log messages from stdin and logs them to a file.
// Usage: app <log_file> <default_severity: low|normal|high>
// Messages can be entered as: [severity] <message>
// If severity is omitted, the default severity is used.
int main(int argc, char* argv[]) {
    // Check command line arguments
    if (argc < 3) {
        std::cout << "Usage: app <log_file> <default_severity: low|normal|high>\n";
        return 1;
    }

    // Parse default severity from command line
    bool ok = false;
    Severity def = parseSeverity(argv[2], ok);
    if (!ok) {
        std::cout << "Invalid severity\n";
        return 1;
    }

    // Initialize file logger with specified log file and default severity
    FileLogger logger(argv[1], def);

    // Thread-safe queue for log messages between main and worker threads
    std::queue<MessageItem> q;
    std::mutex m;
    std::condition_variable cv;
    std::atomic<bool> done{false};

    // Worker thread: processes queued log messages and writes them to the log file
    std::thread worker([&] {
        for (;;) {
            std::unique_lock<std::mutex> lock(m);
            cv.wait(lock, [&] { return done || !q.empty(); });

            while (!q.empty()) {
                auto item = q.front();
                q.pop();
                lock.unlock();
                logger.log(item.text, item.sev);
                lock.lock();
            }

            if (done) break;
        }
    });

    // Print usage instructions
    std::cout << "Enter: <severity(optional)> <message>\n";
    std::cout << "Example: high Disk error\n";
    std::cout << "Example: Hello world\n";
    std::cout << "Type exit to finish\n";

    // Main loop: read user input and queue log messages
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line == "exit") break;

        std::istringstream iss(line);
        std::string first;
        iss >> first;

        Severity sev = logger.defaultSeverity();
        std::string msg;

        if (first == "low" || first == "normal" || first == "high") {
            bool ok2 = false;
            sev = parseSeverity(first, ok2);
            std::getline(iss, msg);
        } else {
            msg = line;
        }

        if (!msg.empty() && msg.front() == ' ') msg.erase(0, 1);
        if (msg.empty()) continue;

        {
            std::lock_guard<std::mutex> lock(m);
            q.push({msg, sev});
        }
        cv.notify_one();
    }

    // Signal worker thread to exit and wait for it to finish
    {
        std::lock_guard<std::mutex> lock(m);
        done = true;
    }
    cv.notify_one();
    worker.join();
    return 0;
}
