#ifndef SYSTEM_CONTROLLER_HPP
#define SYSTEM_CONTROLLER_HPP

#include <httplib.h>
#include <nlohmann/json.hpp>
#include "core/EventStore.hpp"
#include "model/OrderBook.hpp"
#include "model/Account.hpp"
#include "core/StateRebuilder.hpp"
#include "core/Utils.hpp"

class SystemController {
public:
    SystemController(EventStore* eventStore, OrderBook* orderBook, Account* account);
    void registerRoutes(httplib::Server& server);

private:
    EventStore* eventStore;
    OrderBook* orderBook;
    Account* account;

    void handleReplayAll(const httplib::Request& req, httplib::Response& res);
    void handleReplayUntil(const httplib::Request& req, httplib::Response& res);
    void handleGetEvents(const httplib::Request& req, httplib::Response& res);
    void handleGetEventLogFile(const httplib::Request& req, httplib::Response& res);
    void handleGetEventsUntilTimestamp(const std::string& timestampStr, httplib::Response& res);
    };

#endif // SYSTEM_CONTROLLER_HPP
