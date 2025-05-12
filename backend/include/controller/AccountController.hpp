#ifndef ACCOUNT_CONTROLLER_HPP
#define ACCOUNT_CONTROLLER_HPP

#include <httplib.h>
#include <nlohmann/json.hpp>
#include "core/EventStore.hpp"
#include "model/Account.hpp"
#include "model/OrderBook.hpp"
#include "core/StateRebuilder.hpp"
#include "core/Utils.hpp"
#include "event/FundsCredited.hpp"

class AccountController {
public:
    AccountController(EventStore* eventStore, Account* account, OrderBook* orderBook);
    void registerRoutes(httplib::Server& server);

private:
    EventStore* eventStore;
    Account* account;
    OrderBook* orderBook;

    void handleGetAccount(const httplib::Request& req, httplib::Response& res);
    void handleCreditAccount(const httplib::Request& req, httplib::Response& res);
    void handleGetAccountAtTimestamp(const std::string& userId, const std::string& timestampStr, httplib::Response& res);

    };

#endif // ACCOUNT_CONTROLLER_HPP
