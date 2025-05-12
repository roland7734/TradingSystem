#ifndef ORDER_CANCELLED_HPP
#define ORDER_CANCELLED_HPP

#include "core/Event.hpp"
#include "event/OrderCancelled.hpp"
#include "core/OrderType.hpp"


class OrderCancelled : public Event {
public:
    OrderCancelled(std::time_t timestamp,
                   const std::string& userId,
                   const std::string& orderId,
                   OrderType orderType,
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
    OrderType orderType;
    int quantity;
    double price;
};

#endif // ORDER_CANCELLED_HPP
