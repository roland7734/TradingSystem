#include "core/CommandHandlers.hpp"
#include "event/OrderCancelled.hpp"

std::shared_ptr<Event> CommandHandlers::placeOrder(
        const std::string& userId,
        const std::string& orderId,
        OrderType type,
        int quantity,
        double price,
        std::time_t timestamp
) {
    return std::make_shared<OrderPlaced>(timestamp, userId, orderId, type, quantity, price);
}


std::vector<std::shared_ptr<Event>> CommandHandlers::cancelOrder(
        const std::string& userId,
        const std::string& orderId,
        OrderType orderType,
        int quantity,
        double price,
        std::time_t timestamp
) {
    std::vector<std::shared_ptr<Event>> events;
    events.push_back(std::make_shared<OrderCancelled>(timestamp, userId, orderId, orderType, quantity, price));
    return events;
}
