#ifndef FUNDS_CREDITED_HPP
#define FUNDS_CREDITED_HPP

#include "core/Event.hpp"

class FundsCredited : public Event {
public:
    FundsCredited(std::time_t timestamp,
                  const std::string& userId,
                  double amount);

    std::string getType() const override;
    std::string serialize() const override;

    const std::string& getUserId() const;
    double getAmount() const;

private:
    std::string userId;
    double amount;
};

#endif // FUNDS_CREDITED_HPP
