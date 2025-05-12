#include "event/OrderPlaced.hpp"
#include "core/Utils.hpp"
#include <sstream>

OrderPlaced::OrderPlaced(std::time_t timestamp,
                         const std::string& userId,
                         const std::string& orderId,
                         OrderType type,
                         int quantity,
                         double price)
        : Event(timestamp), userId(userId), orderId(orderId),
          type(type), quantity(quantity), price(price) {}

std::string OrderPlaced::getType() const {
    return "OrderPlaced";
}

std::string OrderPlaced::serialize() const {
    std::ostringstream ss;
    ss << "OrderPlaced|"
       << Utils::formatTimestamp(timestamp) << "|"
       << userId << "|"
       << orderId << "|"
       << (type == OrderType::BUY ? "BUY" : "SELL") << "|"
       << "qty:" << quantity << "|"
       << "price:" << price;
    return ss.str();
}


const std::string& OrderPlaced::getUserId() const { return userId; }
const std::string& OrderPlaced::getOrderId() const { return orderId; }
OrderType OrderPlaced::getOrderType() const { return type; }
int OrderPlaced::getQuantity() const { return quantity; }
double OrderPlaced::getPrice() const { return price; }
