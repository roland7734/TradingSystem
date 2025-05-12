#ifndef ORDER_PLACED_HPP
#define ORDER_PLACED_HPP

#include "core/Event.hpp"
#include "core/OrderType.hpp"

class OrderPlaced : public Event {
public:
    OrderPlaced(std::time_t timestamp,
                const std::string& userId,
                const std::string& orderId,
                OrderType type,
                int quantity,
                double price);

    std::string getType() const override;
    std::string serialize() const override;

    const std::string& getUserId() const;
    const std::string& getOrderId() const;
    OrderType getOrderType() const;
    int getQuantity() const;
    double getPrice() const;

private:
    std::string userId;
    std::string orderId;
    OrderType type;
    int quantity;
    double price;
};

#endif // ORDER_PLACED_HPP
