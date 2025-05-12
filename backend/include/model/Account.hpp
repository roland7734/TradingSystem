#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include <unordered_map>
#include <string>
#include <memory>
#include "core/Event.hpp"
#include "event/FundsCredited.hpp"
#include "event/FundsDebited.hpp"

class Account {
public:
    void apply(const std::shared_ptr<Event>& event);

    double getCashBalance(const std::string& userId) const;

    void clear();

private:
    std::unordered_map<std::string, double> cash;
};

#endif // ACCOUNT_HPP
