#ifndef ORDER_BOOK_HPP
#define ORDER_BOOK_HPP

#include <string>
#include <queue>
#include <unordered_map>
#include <vector>
#include <memory>
#include <optional>
#include <ctime>
#include "event/OrderPlaced.hpp"
#include "event/TradeExecuted.hpp"
#include "core/Event.hpp"

struct OrderEntry {
    std::string orderId;
    std::string userId;
    OrderType type;
    int quantity;
    double price;
    std::time_t timestamp;

    bool operator==(const OrderEntry& other) const {
        return orderId == other.orderId;
    }
};

struct MatchResult {
    std::string buyerId;
    std::string sellerId;
    double price;
    int quantity;
    std::string buyOrderId;
    std::string sellOrderId;
};

struct BuyOrderComparator {
    bool operator()(const OrderEntry& a, const OrderEntry& b) const {
        return a.price < b.price || (a.price == b.price && a.timestamp > b.timestamp);
    }
};

struct SellOrderComparator {
    bool operator()(const OrderEntry& a, const OrderEntry& b) const {
        return a.price > b.price || (a.price == b.price && a.timestamp > b.timestamp);
    }
};

class OrderBook {
public:
    void apply(const std::shared_ptr<Event>& event);
    void clear();

    std::vector<MatchResult> tryMatchAll();
    std::vector<MatchResult> tryMatchWithNewOrder(const OrderEntry& newOrder);
    const std::unordered_map<std::string, OrderEntry>& getActiveOrders() const;


private:
    std::priority_queue<OrderEntry, std::vector<OrderEntry>, BuyOrderComparator> buyOrders;
    std::priority_queue<OrderEntry, std::vector<OrderEntry>, SellOrderComparator> sellOrders;
    std::unordered_map<std::string, OrderEntry> orderLookup;
};

#endif // ORDER_BOOK_HPP
