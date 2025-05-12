#include "event/FundsCredited.hpp"
#include "core/Utils.hpp"
#include <sstream>

FundsCredited::FundsCredited(std::time_t timestamp,
                             const std::string& userId,
                             double amount)
        : Event(timestamp), userId(userId), amount(amount) {}

std::string FundsCredited::getType() const {
    return "FundsCredited";
}

std::string FundsCredited::serialize() const {
    std::ostringstream ss;
    ss << "FundsCredited|"
       << Utils::formatTimestamp(timestamp) << "|"
       << userId << "|"
       << amount;
    return ss.str();
}

const std::string& FundsCredited::getUserId() const { return userId; }
double FundsCredited::getAmount() const { return amount; }
