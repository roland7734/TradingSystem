#include "core/StateRebuilder.hpp"

void StateRebuilder::replayAll(EventStore& store, OrderBook& orderBook, Account& account) {
    orderBook.clear();
    account.clear();

    for (const auto& event : store.getAllEvents()) {
        orderBook.apply(event);
        account.apply(event);
    }
}

void StateRebuilder::replayUntil(EventStore& store, OrderBook& orderBook, Account& account, std::time_t cutoff) {
    orderBook.clear();
    account.clear();

    for (const auto& event : store.getEventsUpTo(cutoff)) {
        orderBook.apply(event);
        account.apply(event);
    }
}
