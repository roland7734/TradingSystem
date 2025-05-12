#ifndef TRADING_CONTROLLER_HPP
#define TRADING_CONTROLLER_HPP

#include <httplib.h>
#include <nlohmann/json.hpp>
#include "core/EventStore.hpp"
#include "model/OrderBook.hpp"
#include "model/Account.hpp"
#include "core/CommandHandlers.hpp"
#include "core/StateRebuilder.hpp"

class TradingController {
public:
    TradingController(EventStore* eventStore, OrderBook* orderBook, Account* account);
    void registerRoutes(httplib::Server& server);

private:
    EventStore* eventStore;
    OrderBook* orderBook;
    Account* account;

    void handlePlaceOrder(const httplib::Request& req, httplib::Response& res);
    void handleCancelOrder(const httplib::Request& req, httplib::Response& res);
    void handleGetOrderBook(const httplib::Request& req, httplib::Response& res);
    void handleGetOrderBookAtTimestamp(const std::string& timestampStr, httplib::Response& res);

    };

#endif // TRADING_CONTROLLER_HPP
