#ifndef LOGGER_PROJECT_SOCKET_LOGGER_H
#define LOGGER_PROJECT_SOCKET_LOGGER_H
#include "logger.h"
#include <mutex>
#include <string>

namespace logger {

/**
 * @brief Socket-based logger implementation
 * Sends log messages over TCP socket to remote server
 */
class SocketLogger final : public ILogger {
public:
    /**
     * @brief Construct socket logger
     * @param host Server hostname or IP address
     * @param port Server port number
     * @param defaultSeverity Default severity threshold
     */
    SocketLogger(const std::string& host, unsigned short port, Severity defaultSeverity);
    
    /**
     * @brief Destructor - closes socket connection
     */
    ~SocketLogger() override;

    /**
     * @brief Log a message via socket
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
    [[nodiscard]] Severity defaultSeverity() const override;

private:
    /**
     * @brief Write formatted log record to socket
     * @param r Log record to write
     */
    void writeRecord(const LogRecord& r);

    std::string host_;              // Server hostname or IP
    unsigned short port_;           // Server port
    Severity defaultSeverity_;      // Default severity threshold
    mutable std::mutex mtx_;                // Mutex for thread-safe socket access
};

} // namespace logger

#endif //LOGGER_PROJECT_SOCKET_LOGGER_H
