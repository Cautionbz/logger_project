#include "../include/logger/file_logger.h"
#include <sstream>

namespace logger {

/**
 * @brief Construct file logger
 * @param filename Path to log file (opened in append mode)
 * @param defaultSeverity Default severity threshold
 */
FileLogger::FileLogger(const std::string& filename, Severity defaultSeverity)
    : out_(filename, std::ios::app), defaultSeverity_(defaultSeverity) {}

/**
 * @brief Destructor - closes log file if open
 */
FileLogger::~FileLogger() {
    if (out_.is_open()) out_.close();
}

/**
 * @brief Set default severity threshold (thread-safe)
 * @param s New default severity level
 */
void FileLogger::setDefaultSeverity(Severity s) {
    std::lock_guard<std::mutex> lock(mtx_);
    defaultSeverity_ = s;
}

/**
 * @brief Get current default severity threshold (thread-safe)
 * @return Current default severity
 */
Severity FileLogger::defaultSeverity() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return defaultSeverity_;
}

/**
 * @brief Log a message to file (thread-safe)
 * @param text Message text
 * @param severity Message severity
 */
void FileLogger::log(const std::string& text, Severity severity) {
    std::lock_guard<std::mutex> lock(mtx_);
    // Skip messages below default severity threshold
    if (static_cast<int>(severity) < static_cast<int>(defaultSeverity_)) return;
    writeRecord({text, severity, std::chrono::system_clock::now()});
}

/**
 * @brief Write formatted log record to file
 * @param r Log record to write
 * Format: [timestamp] [severity] message
 */
void FileLogger::writeRecord(const LogRecord& r) {
    out_ << "[" << formatTime(r.time) << "] "
         << "[" << toString(r.severity) << "] "
         << r.text << "\n";
    out_.flush();
}

} // namespace logger
