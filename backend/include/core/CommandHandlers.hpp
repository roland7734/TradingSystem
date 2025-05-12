#ifndef COMMAND_HANDLERS_HPP
#define COMMAND_HANDLERS_HPP

#include <vector>
#include <memory>
#include <string>
#include <ctime>
#include "Event.hpp"
#include "event/OrderPlaced.hpp"

class CommandHandlers {
public:
    static std::shared_ptr<Event> placeOrder(
            const std::string& userId,
            const std::string& orderId,
            OrderType type,
            int quantity,
            double price,
            std::time_t timestamp
    );


    static std::vector<std::shared_ptr<Event>> cancelOrder(
            const std::string& userId,
            const std::string& orderId,
            OrderType orderType,
            int quantity,
            double price,
            std::time_t timestamp
    );
};

#endif // COMMAND_HANDLERS_HPP
