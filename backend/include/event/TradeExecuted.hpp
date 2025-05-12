#ifndef TRADE_EXECUTED_HPP
#define TRADE_EXECUTED_HPP

#include "core/Event.hpp"

class TradeExecuted : public Event {
public:
    TradeExecuted(std::time_t timestamp,
                  const std::string& buyOrderId,
                  const std::string& sellOrderId,
                  double price,
                  int quantity);

    std::string getType() const override;
    std::string serialize() const override;

    const std::string& getBuyOrderId() const;
    const std::string& getSellOrderId() const;
    double getPrice() const;
    int getQuantity() const;

private:
    std::string buyOrderId;
    std::string sellOrderId;
    double price;
    int quantity;
};

#endif // TRADE_EXECUTED_HPP
