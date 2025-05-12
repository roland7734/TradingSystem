#include <fstream>
#include "controller/SystemController.hpp"

SystemController::SystemController(EventStore* eventStore, OrderBook* orderBook, Account* account)
        : eventStore(eventStore), orderBook(orderBook), account(account) {}

void SystemController::registerRoutes(httplib::Server& server) {
    server.Options("/replay/all", [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 204;
    });

    server.Options("/replay/until", [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 204;
    });

    server.Options("/events", [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 204;
    });

    server.Options("/events/log", [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 204;
    });
    server.Options(R"(/events/timestamp/(.+))", [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 204;
    });

    server.Post("/replay/all", [this](const httplib::Request& req, httplib::Response& res) {
        handleReplayAll(req, res);
    });

    server.Post("/replay/until", [this](const httplib::Request& req, httplib::Response& res) {
        handleReplayUntil(req, res);
    });

    server.Get("/events", [this](const httplib::Request& req, httplib::Response& res) {
        handleGetEvents(req, res);
    });

    server.Get("/events/log", [this](const httplib::Request& req, httplib::Response& res) {
        handleGetEventLogFile(req, res);
    });

    server.Get(R"(/events/timestamp/(.+))", [this](const httplib::Request& req, httplib::Response& res) {
        std::string timestampStr = httplib::detail::decode_url(req.matches[1], true);
        handleGetEventsUntilTimestamp(timestampStr, res);
    });

}

void SystemController::handleReplayAll(const httplib::Request&, httplib::Response& res) {
    StateRebuilder::replayAll(*eventStore, *orderBook, *account);
    nlohmann::json json = {
            {"message", "State fully rebuilt from all events"}
    };
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_content(json.dump(), "application/json");
}

void SystemController::handleReplayUntil(const httplib::Request& req, httplib::Response& res) {
    try {
        auto json = nlohmann::json::parse(req.body);
        std::string tsStr = json.at("timestamp");
        std::time_t until = Utils::parseTimestamp(tsStr);

        StateRebuilder::replayUntil(*eventStore, *orderBook, *account, until);

        nlohmann::json response = {
                {"message", "State rebuilt up to specified timestamp"},
                {"timestamp", tsStr}
        };

        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(response.dump(), "application/json");

    } catch (const std::exception& e) {
        nlohmann::json error = {
                {"error", "Failed to replay until timestamp"},
                {"details", e.what()}
        };
        res.status = 400;
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(error.dump(), "application/json");
    }
}
void SystemController::handleGetEvents(const httplib::Request&, httplib::Response& res) {
    auto events = eventStore->getAllEvents();
    nlohmann::json result = nlohmann::json::array();

    for (const auto& e : events) {
        result.push_back(e->serialize());
    }

    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_content(result.dump(2), "application/json");
}

void SystemController::handleGetEventLogFile(const httplib::Request&, httplib::Response& res) {
    std::ifstream file("events.log");
    if (!file.is_open()) {
        res.status = 404;
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content("{\"error\": \"Log file not found\"}", "application/json");
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_content(buffer.str(), "text/plain");
}

void SystemController::handleGetEventsUntilTimestamp(const std::string& timestampStr, httplib::Response& res) {
    try {
        std::time_t timestamp = Utils::parseTimestamp(timestampStr);
        auto filtered = eventStore->getEventsUpTo(timestamp);

        nlohmann::json result = nlohmann::json::array();
        for (const auto& e : filtered) {
            result.push_back(e->serialize());
        }

        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(result.dump(2), "application/json");
    } catch (const std::exception& e) {
        res.status = 400;
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(nlohmann::json{{"error", "Failed to fetch events until timestamp"}, {"details", e.what()}}.dump(), "application/json");
    }
}
