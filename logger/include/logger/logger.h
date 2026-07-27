#ifndef LOGGER_PROJECT_LOGGER_H
#define LOGGER_PROJECT_LOGGER_H

#include <string>
#include <chrono>


namespace logger {

/**
 * @brief Log severity levels
 * Low: Debug/verbose messages
 * Normal: Normal operational messages
 * High: Important/warning/error messages
 */
enum class Severity {
    Low = 0,
    Normal = 1,
    High = 2
};

/**
 * @brief Log record containing message, severity, and timestamp
 */
struct LogRecord {
    std::string text;                          // Log message text
    Severity severity;                         // Severity level
    std::chrono::system_clock::time_point time; // Timestamp
};

/**
 * @brief Abstract base logger interface
 * Defines the interface for all logger implementations
 */
class ILogger {
public:
    virtual ~ILogger() = default;
    
    /**
     * @brief Log a message with specified severity
     * @param text Message text
     * @param severity Severity level (default: Normal)
     */
    virtual void log(const std::string& text, Severity severity = Severity::Normal) = 0;
    
    /**
     * @brief Set default severity threshold
     * @param s New default severity level
     */
    virtual void setDefaultSeverity(Severity s) = 0;
    
    /**
     * @brief Get current default severity threshold
     * @return Current default severity
     */
    virtual Severity defaultSeverity() const = 0;
};

/**
 * @brief Convert Severity enum to string representation
 * @param s Severity level
 * @return String representation ("Low", "Normal", "High")
 */
std::string toString(Severity s);

/**
 * @brief Format timestamp to string
 * @param tp Time point to format
 * @return Formatted timestamp string
 */
std::string formatTime(std::chrono::system_clock::time_point tp);

} // namespace logger

#endif //LOGGER_PROJECT_LOGGER_H
