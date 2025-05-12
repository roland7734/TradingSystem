#include "core/Utils.hpp"
#include <chrono>
#include <sstream>
#include <iomanip>
#include <random>

std::time_t Utils::getCurrentTimestamp() {
    return std::time(nullptr);
}

std::string Utils::generateOrderId() {
    static std::mt19937_64 rng(std::random_device{}());
    static std::uniform_int_distribution<uint64_t> dist(100000, 999999);
    return "ORD" + std::to_string(dist(rng));
}

std::string Utils::formatTimestamp(std::time_t ts) {
    std::tm* tm_ptr = std::localtime(&ts);
    std::ostringstream ss;
    ss << std::put_time(tm_ptr, "%d/%m/%Y %H:%M:%S");
    return ss.str();
}

std::time_t Utils::parseTimestamp(const std::string& str) {
    std::tm tm = {};
    tm.tm_isdst = 1;
    std::istringstream ss(str);
    ss >> std::get_time(&tm, "%d/%m/%Y %H:%M:%S");
    return std::mktime(&tm);
}
