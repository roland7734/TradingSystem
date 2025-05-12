#ifndef FUNDS_DEBITED_HPP
#define FUNDS_DEBITED_HPP

#include "core/Event.hpp"

class FundsDebited : public Event {
public:
    FundsDebited(std::time_t timestamp,
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

#endif // FUNDS_DEBITED_HPP
