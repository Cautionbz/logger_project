#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "logger/file_logger.h"
#include <fstream>
#include <filesystem>
#include <thread>

using namespace logger;

static const char* TEST_LOG_FILE = "test_logger_output.log";

TEST_GROUP(FileLogger) {
    void setup() {
        // Remove test file if exists
        std::filesystem::remove(TEST_LOG_FILE);
    }
    
    void teardown() {
        std::filesystem::remove(TEST_LOG_FILE);
    }
};

TEST(FileLogger, ConstructorCreatesFile) {
    FileLogger logger(TEST_LOG_FILE, Severity::Normal);
    // File should be created (in append mode)
}

TEST(FileLogger, DefaultSeverityIsNormal) {
    FileLogger logger(TEST_LOG_FILE, Severity::Normal);
    CHECK_EQUAL(static_cast<int>(Severity::Normal), static_cast<int>(logger.defaultSeverity()));
}

TEST(FileLogger, SetDefaultSeverity) {
    FileLogger logger(TEST_LOG_FILE, Severity::Low);
    CHECK_EQUAL(static_cast<int>(Severity::Low), static_cast<int>(logger.defaultSeverity()));
    
    logger.setDefaultSeverity(Severity::High);
    CHECK_EQUAL(static_cast<int>(Severity::High), static_cast<int>(logger.defaultSeverity()));
}

TEST(FileLogger, LogBelowDefaultSeverityIsSkipped) {
    FileLogger logger(TEST_LOG_FILE, Severity::High);
    
    // Low severity should be skipped (below High)
    logger.log("Low message", Severity::Low);
    logger.log("Normal message", Severity::Normal);
    
    std::ifstream file(TEST_LOG_FILE);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    STRCMP_CONTAINS("", content.c_str()); // Should be empty
}

TEST(FileLogger, LogAtOrAboveDefaultSeverityIsWritten) {
    FileLogger logger(TEST_LOG_FILE, Severity::Normal);
    
    logger.log("High message", Severity::High);
    logger.log("Normal message", Severity::Normal);
    // Low should be skipped
    logger.log("Low message", Severity::Low);
    
    std::ifstream file(TEST_LOG_FILE);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    STRCMP_CONTAINS("High message", content.c_str());
    STRCMP_CONTAINS("Normal message", content.c_str());
}

TEST(FileLogger, LogFormatContainsTimestampSeverityMessage) {
    FileLogger logger(TEST_LOG_FILE, Severity::Low);
    
    logger.log("Test message", Severity::High);
    
    std::ifstream file(TEST_LOG_FILE);
    std::string line;
    std::getline(file, line);
    
    // Check format: [timestamp] [severity] message
    CHECK(line.find("[") == 0); // Starts with [
    CHECK(line.find("] [") != std::string::npos); // Has ] [
    CHECK(line.find("High]") != std::string::npos); // Has severity
    CHECK(line.find("Test message") != std::string::npos); // Has message
}

TEST(FileLogger, ThreadSafetyMultipleThreads) {
    FileLogger logger(TEST_LOG_FILE, Severity::Low);
    const int num_threads = 4;
    const int messages_per_thread = 100;
    
    auto log_messages = [&](int thread_id) {
        for (int i = 0; i < messages_per_thread; ++i) {
            logger.log("Thread " + std::to_string(thread_id) + " msg " + std::to_string(i), Severity::Normal);
        }
    };
    
    std::thread threads[num_threads];
    for (int i = 0; i < num_threads; ++i) {
        threads[i] = std::thread(log_messages, i);
    }
    
    for (int i = 0; i < num_threads; ++i) {
        threads[i].join();
    }
    
    // Count lines in file
    std::ifstream file(TEST_LOG_FILE);
    int line_count = 0;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) line_count++;
    }
    
    CHECK_EQUAL(num_threads * messages_per_thread, line_count);
}

TEST(FileLogger, DefaultSeverityInLogCall) {
    FileLogger logger(TEST_LOG_FILE, Severity::Low);
    
    // Call log without severity (uses default)
    logger.log("Default severity message");
    
    std::ifstream file(TEST_LOG_FILE);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    STRCMP_CONTAINS("Default severity message", content.c_str());
    STRCMP_CONTAINS("Normal]", content.c_str()); // Default is Normal
}