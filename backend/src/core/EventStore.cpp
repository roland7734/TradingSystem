#include "core/EventStore.hpp"
#include "event/OrderPlaced.hpp"
#include "event/OrderCancelled.hpp"
#include "event/TradeExecuted.hpp"
#include "event/FundsDebited.hpp"
#include "event/FundsCredited.hpp"
#include "core/Utils.hpp"
#include <fstream>
#include <sstream>

EventStore::EventStore(const std::string& filename) : logFile(filename) {
    loadFromFile();
}

void EventStore::append(std::shared_ptr<Event> event) {
    events.push_back(event);
    appendToFile(event);
}

void EventStore::appendToFile(const std::shared_ptr<Event>& event) {
    std::ofstream out(logFile, std::ios::app);
    if (out.is_open()) {
        out << event->serialize() << "\n";
        out.close();
    }
}

std::vector<std::shared_ptr<Event>> EventStore::getAllEvents() {
    clear();
    loadFromFile();
    return events;
}

std::vector<std::shared_ptr<Event>> EventStore::getEventsUpTo(std::time_t timestamp) {
    clear();
    loadFromFile();
    std::vector<std::shared_ptr<Event>> result;
    for (const auto& e : events) {
        if (e->getTimestamp() <= timestamp) {
            result.push_back(e);
        } else {
            break;
        }
    }
    return result;
}

void EventStore::clear() {
    events.clear();
}

void EventStore::loadFromFile() {
    events.clear();
    std::ifstream in(logFile);
    std::string line;

    while (std::getline(in, line)) {
        std::istringstream ss(line);
        std::string type;
        std::getline(ss, type, '|');

        std::shared_ptr<Event> event;

        if (type == "OrderPlaced") {
            std::string userId, orderId, side, quantityStr, priceStr;
            std::string timeStr;
            std::getline(ss, timeStr, '|');
            std::time_t ts = Utils::parseTimestamp(timeStr);
            std::getline(ss, userId, '|');
            std::getline(ss, orderId, '|');
            std::getline(ss, side, '|');
            std::getline(ss, quantityStr, '|');
            std::getline(ss, priceStr);

            int quantity = std::stoi(quantityStr.substr(quantityStr.find(':') + 1));
            double price = std::stod(priceStr.substr(priceStr.find(':') + 1));

            event = std::make_shared<OrderPlaced>(
                    ts, userId, orderId,
                    side == "BUY" ? OrderType::BUY : OrderType::SELL,
                    quantity, price);
        }
        else if (type == "OrderCancelled") {
            std::string timeStr, userId, orderId, orderTypeStr, quantityStr, priceStr;

            std::getline(ss, timeStr, '|');
            std::time_t ts = Utils::parseTimestamp(timeStr);

            std::getline(ss, userId, '|');
            std::getline(ss, orderId, '|');
            std::getline(ss, orderTypeStr, '|');
            std::getline(ss, quantityStr, '|');
            std::getline(ss, priceStr);

            int quantity = std::stoi(quantityStr.substr(quantityStr.find(':') + 1));
            double price = std::stod(priceStr.substr(priceStr.find(':') + 1));

            OrderType orderType = (orderTypeStr == "BUY") ? OrderType::BUY : OrderType::SELL;

            event = std::make_shared<OrderCancelled>(ts, userId, orderId, orderType, quantity, price);
        }

        else if (type == "FundsDebited") {
            std::string userId;
            double amount;
            std::string timeStr;
            std::getline(ss, timeStr, '|');
            std::time_t ts = Utils::parseTimestamp(timeStr);
            std::getline(ss, userId, '|');
            ss >> amount;
            event = std::make_shared<FundsDebited>(ts, userId, amount);
        }
        else if (type == "FundsCredited") {
            std::string userId;
            double amount;
            std::string timeStr;
            std::getline(ss, timeStr, '|');
            std::time_t ts = Utils::parseTimestamp(timeStr);
            std::getline(ss, userId, '|');
            ss >> amount;
            event = std::make_shared<FundsCredited>(ts, userId, amount);
        }
        else if (type == "TradeExecuted") {
            std::string buyId, sellId, priceStr, quantityStr;
            std::string timeStr;
            std::getline(ss, timeStr, '|');
            std::time_t ts = Utils::parseTimestamp(timeStr);
            std::getline(ss, buyId, '|');
            std::getline(ss, sellId, '|');
            std::getline(ss, quantityStr, '|');
            std::getline(ss, priceStr);

            int quantity = std::stoi(quantityStr.substr(quantityStr.find(':') + 1));
            double price = std::stod(priceStr.substr(priceStr.find(':') + 1));
            event = std::make_shared<TradeExecuted>(ts, buyId, sellId, price, quantity);
        }

        if (event) {
            events.push_back(event);
        }
    }
    in.close();
}
