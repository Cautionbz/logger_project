#include "CppUTest/TestHarness.h"
#include "logger/logger.h"
#include <string>
#include <chrono>

using namespace logger;

TEST_GROUP(LoggerCommon) {
};

TEST(LoggerCommon, SeverityEnumValues) {
    CHECK_EQUAL(0, static_cast<int>(Severity::Low));
    CHECK_EQUAL(1, static_cast<int>(Severity::Normal));
    CHECK_EQUAL(2, static_cast<int>(Severity::High));
}

TEST(LoggerCommon, SeverityToString) {
    STRCMP_EQUAL("Low", toString(Severity::Low).c_str());
    STRCMP_EQUAL("Normal", toString(Severity::Normal).c_str());
    STRCMP_EQUAL("High", toString(Severity::High).c_str());
}

TEST(LoggerCommon, FormatTime) {
    auto now = std::chrono::system_clock::now();
    std::string formatted = formatTime(now);
    // Format: YYYY-MM-DD HH:MM:SS
    CHECK_EQUAL(19, formatted.length());
    CHECK(formatted[4] == '-');
    CHECK(formatted[7] == '-');
    CHECK(formatted[10] == ' ');
    CHECK(formatted[13] == ':');
    CHECK(formatted[16] == ':');
}

TEST(LoggerCommon, LogRecordStructure) {
    LogRecord record;
    record.text = "Test message";
    record.severity = Severity::High;
    record.time = std::chrono::system_clock::now();
    
    STRCMP_EQUAL("Test message", record.text.c_str());
    CHECK_EQUAL(static_cast<int>(Severity::High), static_cast<int>(record.severity));
}