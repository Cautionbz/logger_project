#ifndef LOGGER_PROJECT_FILE_LOGGER_H
#define LOGGER_PROJECT_FILE_LOGGER_H
#include "logger.h"
#include <fstream>
#include <mutex>

namespace logger {

/**
 * @brief File-based logger implementation
 * Writes log messages to a file with thread-safe operations
 */
class FileLogger final : public ILogger {
public:
    /**
     * @brief Construct file logger
     * @param filename Path to log file
     * @param defaultSeverity Default severity threshold
     */
    FileLogger(const std::string& filename, Severity defaultSeverity);
    
    /**
     * @brief Destructor - closes the log file
     */
    ~FileLogger() override;

    /**
     * @brief Log a message to file
     * @param text Message text
     * @param severity Message severity (default: Normal)
     */
    void log(const std::string& text, Severity severity = Severity::Normal) override;
    
    /**
     * @brief Set default severity threshold
     * @param s New default severity level
     */
    void setDefaultSeverity(Severity s) override;
    
    /**
     * @brief Get current default severity threshold
     * @return Current default severity
     */
    Severity defaultSeverity() const override;

private:
    /**
     * @brief Write formatted log record to file
     * @param r Log record to write
     */
    void writeRecord(const LogRecord& r);

    std::ofstream out_;              // Output file stream
    mutable std::mutex mtx_;         // Mutex for thread-safe file access
    Severity defaultSeverity_;       // Default severity threshold
};

} // namespace logger

#endif //LOGGER_PROJECT_FILE_LOGGER_H
