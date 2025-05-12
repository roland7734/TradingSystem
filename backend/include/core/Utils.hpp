#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <ctime>

class Utils {
public:
    static std::time_t getCurrentTimestamp();
    static std::string generateOrderId();
    static std::string formatTimestamp(std::time_t ts);
    static std::time_t parseTimestamp(const std::string& str);
};

#endif // UTILS_HPP
