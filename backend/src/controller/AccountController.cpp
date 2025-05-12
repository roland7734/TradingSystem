#include "controller/AccountController.hpp"

AccountController::AccountController(EventStore* eventStore, Account* account, OrderBook* orderBook)
        : eventStore(eventStore), account(account), orderBook(orderBook) {}

void AccountController::registerRoutes(httplib::Server& server) {
    server.Options("/accounts/credit", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 204;
    });

    server.Options(R"(/accounts/([a-zA-Z0-9_]+))", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 204;
    });

    server.Options(R"(/accounts/([a-zA-Z0-9_]+)/timestamp/(.+))", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 204;
    });

    server.Post("/accounts/credit", [this](const httplib::Request& req, httplib::Response& res) {
        handleCreditAccount(req, res);
    });

    server.Get(R"(/accounts/([a-zA-Z0-9_]+))", [this](const httplib::Request& req, httplib::Response& res) {
        handleGetAccount(req, res);
    });

    server.Get(R"(/accounts/([a-zA-Z0-9_]+)/timestamp/(.+))", [this](const httplib::Request& req, httplib::Response& res) {
        std::string userId = req.matches[1];
        std::string timestampStr = httplib::detail::decode_url(req.matches[2], true);
        handleGetAccountAtTimestamp(userId, timestampStr, res);
    });

}

void AccountController::handleCreditAccount(const httplib::Request& req, httplib::Response& res) {
    try {
        auto body = nlohmann::json::parse(req.body);
        std::string userId = body["user_id"];
        double amount = body["amount"];

        auto timestamp = Utils::getCurrentTimestamp();
        auto event = std::make_shared<FundsCredited>(timestamp, userId, amount);
        eventStore->append(event);

        StateRebuilder::replayAll(*eventStore, *orderBook, *account);

        nlohmann::json response = {
                {"message", "Funds credited"},
                {"user_id", userId},
                {"new_balance", account->getCashBalance(userId)}
        };

        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(response.dump(), "application/json");

    } catch (const std::exception& e) {
        nlohmann::json error = {
                {"error", "Failed to credit account"},
                {"details", e.what()}
        };
        res.status = 400;
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(error.dump(), "application/json");
    }
}

void AccountController::handleGetAccount(const httplib::Request& req, httplib::Response& res) {
    try {
        std::string userId = req.matches[1];

        StateRebuilder::replayAll(*eventStore, *orderBook, *account);
        double cash = account->getCashBalance(userId);

        nlohmann::json response = {
                {"user_id", userId},
                {"cash_balance", cash}
        };

        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(response.dump(), "application/json");
    } catch (const std::exception& e) {
        nlohmann::json error = {
                {"error", "Failed to get account info"},
                {"details", e.what()}
        };
        res.status = 400;
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(error.dump(), "application/json");
    }
}


void AccountController::handleGetAccountAtTimestamp(const std::string& userId, const std::string& timestampStr, httplib::Response& res) {
    try {
        std::time_t timestamp = Utils::parseTimestamp(timestampStr);
        OrderBook tempOrderBook;
        Account tempAccount;
        StateRebuilder::replayUntil(*eventStore, tempOrderBook, tempAccount, timestamp);

        double cash = tempAccount.getCashBalance(userId);

        nlohmann::json response = {
                {"user_id", userId},
                {"cash_balance", cash}
        };

        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(response.dump(), "application/json");
    } catch (const std::exception& e) {
        res.status = 400;
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(nlohmann::json{{"error", "Failed to fetch timestamped balance"}, {"details", e.what()}}.dump(), "application/json");
    }
}