#include "event/FundsDebited.hpp"
#include "core/Utils.hpp"
#include <sstream>

FundsDebited::FundsDebited(std::time_t timestamp,
                           const std::string& userId,
                           double amount)
        : Event(timestamp), userId(userId), amount(amount) {}

std::string FundsDebited::getType() const {
    return "FundsDebited";
}

std::string FundsDebited::serialize() const {
    std::ostringstream ss;
    ss << "FundsDebited|"
       << Utils::formatTimestamp(timestamp) << "|"
       << userId << "|"
       << amount;
    return ss.str();
}

const std::string& FundsDebited::getUserId() const { return userId; }
double FundsDebited::getAmount() const { return amount; }
