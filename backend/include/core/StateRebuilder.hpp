#ifndef STATE_REBUILDER_HPP
#define STATE_REBUILDER_HPP

#include <memory>
#include "EventStore.hpp"
#include "model/OrderBook.hpp"
#include "model/Account.hpp"

class StateRebuilder {
public:
    static void replayAll(EventStore& store, OrderBook& orderBook, Account& account);
    static void replayUntil(EventStore& store, OrderBook& orderBook, Account& account, std::time_t cutoff);
};

#endif // STATE_REBUILDER_HPP
