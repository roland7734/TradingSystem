#include <iostream>
#include "controller/TradingController.hpp"
#include "core/Utils.hpp"

TradingController::TradingController(EventStore* eventStore, OrderBook* orderBook, Account* account)
        : eventStore(eventStore), orderBook(orderBook), account(account) {}

void TradingController::registerRoutes(httplib::Server& server) {
    server.Options("/orders", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 204;
    });

    server.Options("/orders/(.*)", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 204;
    });

    server.Options("/orderbook", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 204;
    });

    server.Options(R"(/orderbook/timestamp/(.+))", [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 204;
    });

    server.Post("/orders", [this](const httplib::Request& req, httplib::Response& res) {
        handlePlaceOrder(req, res);
    });

    server.Delete(R"(/orders/([a-zA-Z0-9_]+))", [this](const httplib::Request& req, httplib::Response& res) {
        handleCancelOrder(req, res);
    });

    server.Get("/orderbook", [this](const httplib::Request& req, httplib::Response& res) {
        handleGetOrderBook(req, res);
    });

    server.Get(R"(/orderbook/timestamp/(.+))", [this](const httplib::Request& req, httplib::Response& res) {
        std::string timestampStr = httplib::detail::decode_url(req.matches[1], true);
        handleGetOrderBookAtTimestamp(timestampStr, res);
    });

}

void TradingController::handlePlaceOrder(const httplib::Request& req, httplib::Response& res) {
    try {
        auto json_body = nlohmann::json::parse(req.body);
        std::string userId = json_body["user_id"];
        std::string typeStr = json_body["type"];
        int quantity = json_body["quantity"];
        double price = json_body["price"];

        OrderType type = (typeStr == "BUY") ? OrderType::BUY : OrderType::SELL;
        std::string orderId = Utils::generateOrderId();
        auto timestamp = Utils::getCurrentTimestamp();

        std::vector<std::shared_ptr<Event>> events;

        double totalCost = quantity * price;

        if (type == OrderType::BUY) {
            if (account->getCashBalance(userId) < totalCost) {
                throw std::runtime_error("Insufficient funds to place BUY order");
            }
            events.push_back(std::make_shared<FundsDebited>(timestamp, userId, totalCost));
        }
        auto eventPlaceOrder = CommandHandlers::placeOrder(userId, orderId, type, quantity, price, timestamp);
        events.push_back(eventPlaceOrder);

        for (auto& event : events) {
            eventStore->append(event);
        }

        StateRebuilder::replayAll(*eventStore, *orderBook, *account);
        OrderEntry newOrder{orderId, userId, type, quantity, price, timestamp};
        std::vector<MatchResult> matches = orderBook->tryMatchWithNewOrder(newOrder);

        events.clear();

        double spent = 0.0;
        int soldQuantity = 0;
        for (const auto& match : matches) {
            double matchTotal = match.price * match.quantity;
            soldQuantity += match.quantity;
            spent += (match.buyerId == userId) ? matchTotal : 0;

            events.push_back(std::make_shared<FundsCredited>(timestamp, match.sellerId, matchTotal));
            events.push_back(std::make_shared<TradeExecuted>(timestamp, match.buyOrderId, match.sellOrderId, match.price, match.quantity));
        }

        if (type == OrderType::BUY) {
            double refund = price * soldQuantity - spent;
            if (refund > 0.0) {
                events.push_back(std::make_shared<FundsCredited>(timestamp, userId, refund));
            }
        }

        for (auto& event : events) {
            eventStore->append(event);
        }

        nlohmann::json resp = {
                {"message", "Order processed"},
                {"order_id", orderId}
        };

        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(resp.dump(), "application/json");

    } catch (const std::exception& e) {
        nlohmann::json error = {
                {"error", "Order failed"},
                {"details", e.what()}
        };
        res.status = 400;
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(error.dump(), "application/json");
    }
}


void TradingController::handleCancelOrder(const httplib::Request& req, httplib::Response& res) {
    try {
        std::string orderId = req.matches[1];
        std::string userId = req.get_param_value("user_id");
        auto timestamp = Utils::getCurrentTimestamp();

        const auto& orderMap = orderBook->getActiveOrders();
        auto it = orderMap.find(orderId);

        if (it == orderMap.end() || it->second.userId != userId) {
            throw std::runtime_error("Order not found or access denied");
        }

        const auto& order = it->second;

        auto events = CommandHandlers::cancelOrder(userId, orderId,
                                                   order.type, order.quantity, order.price, timestamp);

        for (auto& event : events) {
            eventStore->append(event);
        }

        nlohmann::json resp = {
                {"message", "Order cancelled"},
                {"order_id", orderId}
        };

        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(resp.dump(), "application/json");
    } catch (const std::exception& e) {
        nlohmann::json error = {
                {"error", "Failed to cancel order"},
                {"details", e.what()}
        };
        res.status = 400;
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(error.dump(), "application/json");
    }
}

void TradingController::handleGetOrderBook(const httplib::Request& req, httplib::Response& res) {
    try {
        nlohmann::json result = nlohmann::json::array();
        StateRebuilder::replayAll(*eventStore, *orderBook, *account);
        const auto& orders = orderBook->getActiveOrders();

        for (const auto& [orderId, order] : orders) {
            result.push_back({
                                     {"order_id", orderId},
                                     {"user_id", order.userId},
                                     {"type", order.type == OrderType::BUY ? "BUY" : "SELL"},
                                     {"quantity", order.quantity},
                                     {"price", order.price},
                                     {"timestamp", Utils::formatTimestamp(order.timestamp)}
                             });
        }

        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(result.dump(), "application/json");
    } catch (const std::exception& e) {
        nlohmann::json error = {
                {"error", "Failed to retrieve order book"},
                {"details", e.what()}
        };
        res.status = 500;
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(error.dump(), "application/json");
    }
}

void TradingController::handleGetOrderBookAtTimestamp(const std::string& timestampStr, httplib::Response& res) {
    try {
        std::time_t timestamp = Utils::parseTimestamp(timestampStr);

        OrderBook tempOrderBook;
        Account tempAccount;
        StateRebuilder::replayUntil(*eventStore, tempOrderBook, tempAccount, timestamp);

        nlohmann::json result = nlohmann::json::array();
        for (const auto& [orderId, order] : tempOrderBook.getActiveOrders()) {
            result.push_back({
                                     {"order_id", orderId},
                                     {"user_id", order.userId},
                                     {"type", order.type == OrderType::BUY ? "BUY" : "SELL"},
                                     {"quantity", order.quantity},
                                     {"price", order.price},
                                     {"timestamp", Utils::formatTimestamp(order.timestamp)}

                             });
        }

        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(result.dump(), "application/json");
    } catch (const std::exception& e) {
        res.status = 400;
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(nlohmann::json{{"error", "Failed to get order book at timestamp"}, {"details", e.what()}}.dump(), "application/json");
    }
}
