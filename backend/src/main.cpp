#include <iostream>
#include <httplib.h>
#include "controller/TradingController.hpp"
#include "controller/AccountController.hpp"
#include "controller/SystemController.hpp"
#include "core/StateRebuilder.hpp"
#include "core/EventStore.hpp"
#include "model/OrderBook.hpp"
#include "model/Account.hpp"

int main() {
    EventStore eventStore("events.log");
    OrderBook orderBook;
    Account account;

    StateRebuilder::replayAll(eventStore, orderBook, account);

    httplib::Server server;

    TradingController tradingController(&eventStore, &orderBook, &account);
    AccountController accountController(&eventStore, &account, &orderBook);
    SystemController systemController(&eventStore, &orderBook, &account);

    tradingController.registerRoutes(server);
    accountController.registerRoutes(server);
    systemController.registerRoutes(server);

    std::cout << " Server is running on http://localhost:8080\n";
    server.listen("0.0.0.0", 8080);

    return 0;
}
