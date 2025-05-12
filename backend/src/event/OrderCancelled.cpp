#include "event/OrderCancelled.hpp"
#include "core/Utils.hpp"
#include <sstream>

OrderCancelled::OrderCancelled(std::time_t timestamp,
                               const std::string& userId,
                               const std::string& orderId,
                               OrderType orderType,
                               int quantity,
                               double price)
        : Event(timestamp),
          userId(userId),
          orderId(orderId),
          orderType(orderType),
          quantity(quantity),
          price(price) {}

std::string OrderCancelled::getType() const {
    return "OrderCancelled";
}

std::string OrderCancelled::serialize() const {
    std::ostringstream ss;
    ss << "OrderCancelled|"
       << Utils::formatTimestamp(timestamp) << "|"
       << userId << "|"
       << orderId << "|"
       << (orderType == OrderType::BUY ? "BUY" : "SELL") << "|"
       << "qty:" << quantity << "|"
       << "price:" << price;
    return ss.str();
}

const std::string& OrderCancelled::getUserId() const { return userId; }
const std::string& OrderCancelled::getOrderId() const { return orderId; }
OrderType OrderCancelled::getOrderType() const { return orderType; }
int OrderCancelled::getQuantity() const { return quantity; }
double OrderCancelled::getPrice() const { return price; }
