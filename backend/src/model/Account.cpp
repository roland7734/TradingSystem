#include "model/Account.hpp"
#include "event/OrderCancelled.hpp"

void Account::apply(const std::shared_ptr<Event>& event) {
    if (event->getType() == "FundsCredited") {
        auto e = std::dynamic_pointer_cast<FundsCredited>(event);
        if (e) {
            cash[e->getUserId()] += e->getAmount();
        }

    } else if (event->getType() == "FundsDebited") {
        auto e = std::dynamic_pointer_cast<FundsDebited>(event);
        if (e) {
            cash[e->getUserId()] -= e->getAmount();
        }

    } else if (event->getType() == "OrderCancelled") {
        auto e = std::dynamic_pointer_cast<OrderCancelled>(event);
        if (e && e->getOrderType() == OrderType::BUY) {
            double refund = e->getQuantity() * e->getPrice();
            cash[e->getUserId()] += refund;
        }
    }
}

double Account::getCashBalance(const std::string& userId) const {
    auto it = cash.find(userId);
    return (it != cash.end()) ? it->second : 0.0;
}

void Account::clear() {
    cash.clear();
}
