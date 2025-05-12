#include "event/TradeExecuted.hpp"
#include "core/Utils.hpp"
#include <sstream>

TradeExecuted::TradeExecuted(std::time_t timestamp,
                             const std::string& buyOrderId,
                             const std::string& sellOrderId,
                             double price,
                             int quantity)
        : Event(timestamp), buyOrderId(buyOrderId), sellOrderId(sellOrderId),
          price(price), quantity(quantity) {}

std::string TradeExecuted::getType() const {
    return "TradeExecuted";
}

std::string TradeExecuted::serialize() const {
    std::ostringstream ss;
    ss << "TradeExecuted|"
       << Utils::formatTimestamp(timestamp) << "|"
       << buyOrderId << "|"
       << sellOrderId << "|"
       << "qty:" << quantity << "|"
       << "price:" << price;
    return ss.str();
}

const std::string& TradeExecuted::getBuyOrderId() const { return buyOrderId; }
const std::string& TradeExecuted::getSellOrderId() const { return sellOrderId; }
double TradeExecuted::getPrice() const { return price; }
int TradeExecuted::getQuantity() const { return quantity; }
