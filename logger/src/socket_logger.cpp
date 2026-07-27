#include "../include/logger/socket_logger.h"
#include <sstream>
#include <iostream>

#if defined(_WIN32)
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")
    using socket_type = SOCKET;
    static void close_socket(socket_type s) { closesocket(s); }
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    using socket_type = int;
    static void close_socket(socket_type s) { close(s); }
#endif

namespace logger {

/**
 * @brief Send all data reliably over socket
 * @param s Socket descriptor
 * @param data Data to send
 * @return true if all data sent successfully, false on error
 */
static bool sendAll(socket_type s, const std::string& data) {
    const char* p = data.c_str();
    size_t left = data.size();
    while (left > 0) {
#if defined(_WIN32)
        int sent = ::send(s, p, static_cast<int>(left), 0);
#else
        ssize_t sent = ::send(s, p, left, 0);
#endif
        if (sent <= 0) return false;
        p += sent;
        left -= static_cast<size_t>(sent);
    }
    return true;
}

/**
 * @brief Construct socket logger
 * @param host Server hostname/IP address
 * @param port Server port number
 * @param defaultSeverity Default severity threshold
 * Initializes Winsock on Windows
 */
SocketLogger::SocketLogger(const std::string& host, unsigned short port, Severity defaultSeverity)
    : host_(host), port_(port), defaultSeverity_(defaultSeverity) {
#if defined(_WIN32)
    WSADATA wsa{};
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
}

/**
 * @brief Destructor - cleans up Winsock on Windows
 */
SocketLogger::~SocketLogger() {
#if defined(_WIN32)
    WSACleanup();
#endif
}

/**
 * @brief Set default severity threshold (thread-safe)
 * @param s New default severity level
 */
void SocketLogger::setDefaultSeverity(Severity s) {
    std::lock_guard<std::mutex> lock(mtx_);
    defaultSeverity_ = s;
}

/**
 * @brief Get current default severity threshold (thread-safe)
 * @return Current default severity
 */
Severity SocketLogger::defaultSeverity() const  {
    std::lock_guard<std::mutex> lock(mtx_);
    return defaultSeverity_;
}

/**
 * @brief Log a message to socket server (thread-safe)
 * @param text Message text
 * @param severity Message severity
 * Skips messages below default severity threshold
 */
void SocketLogger::log(const std::string& text, Severity severity) {
    std::lock_guard<std::mutex> lock(mtx_);
    if (static_cast<int>(severity) < static_cast<int>(defaultSeverity_)) return;
    writeRecord({text, severity, std::chrono::system_clock::now()});
}

/**
 * @brief Write formatted log record to socket server
 * @param r Log record to send
 * Creates new connection for each log message
 */
void SocketLogger::writeRecord(const LogRecord& r) {
    socket_type s = ::socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) return;

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);
    inet_pton(AF_INET, host_.c_str(), &addr.sin_addr);

    if (::connect(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0) {
        close_socket(s);
        return;
    }

    std::ostringstream oss;
    oss << "[" << formatTime(r.time) << "] "
        << "[" << toString(r.severity) << "] "
        << r.text << "\n";

    sendAll(s, oss.str());
    close_socket(s);
}

} // namespace logger
