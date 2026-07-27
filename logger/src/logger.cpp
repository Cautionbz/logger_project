#include "../include/logger/logger.h"
#include <sstream>
#include <iomanip>

namespace logger {

/**
 * @brief Convert Severity enum to string representation
 * @param s Severity value
 * @return String representation ("Low", "Normal", "High")
 */
std::string toString(Severity s) {
    switch (s) {
        case Severity::Low: return "Low";
        case Severity::Normal: return "Normal";
        case Severity::High: return "High";
    }
    return "Normal";
}

/**
 * @brief Format time point as string
 * @param tp Time point to format
 * @return Formatted string (YYYY-MM-DD HH:MM:SS)
 */
std::string formatTime(std::chrono::system_clock::time_point tp) {
    auto t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm{};

#if defined(_WIN32)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

} // namespace logger
