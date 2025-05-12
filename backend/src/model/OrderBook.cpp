#include "model/OrderBook.hpp"
#include "event/OrderCancelled.hpp"

void OrderBook::apply(const std::shared_ptr<Event>& event) {
    if (event->getType() == "OrderPlaced") {
        auto e = std::dynamic_pointer_cast<OrderPlaced>(event);
        if (e) {
            OrderEntry entry {
                    e->getOrderId(),
                    e->getUserId(),
                    e->getOrderType(),
                    e->getQuantity(),
                    e->getPrice(),
                    e->getTimestamp()
            };

            if (entry.type == OrderType::BUY) {
                buyOrders.push(entry);
            } else {
                sellOrders.push(entry);
            }

            orderLookup[entry.orderId] = entry;
        }
    } else if (event->getType() == "OrderCancelled") {
        auto e = std::dynamic_pointer_cast<OrderCancelled>(event);
        if (e) {
            orderLookup.erase(e->getOrderId());
        }
    } else if (event->getType() == "TradeExecuted") {
        auto e = std::dynamic_pointer_cast<TradeExecuted>(event);
        if (e) {
            auto updateQty = [&](const std::string& orderId, int tradedQty) {
                auto it = orderLookup.find(orderId);
                if (it != orderLookup.end()) {
                    OrderEntry updated = it->second;
                    updated.quantity -= tradedQty;
                    if (updated.quantity <= 0) {
                        orderLookup.erase(orderId);
                    } else {
                        orderLookup[orderId] = updated;
                    }
                }
            };

            updateQty(e->getBuyOrderId(), e->getQuantity());
            updateQty(e->getSellOrderId(), e->getQuantity());
        }
    }
}
std::vector<MatchResult> OrderBook::tryMatchWithNewOrder(const OrderEntry& newOrder) {
    std::vector<MatchResult> results;

    OrderEntry incoming = newOrder;

    auto priceMatch = [&](const OrderEntry& existing) {
        return (incoming.type == OrderType::BUY) ? (incoming.price >= existing.price)
                                                 : (incoming.price <= existing.price);
    };

    if (incoming.type == OrderType::BUY) {
        while (!sellOrders.empty() && incoming.quantity > 0) {
            OrderEntry top = sellOrders.top();

            if (orderLookup.find(top.orderId) == orderLookup.end()) {
                sellOrders.pop();
                continue;
            }

            top = orderLookup[top.orderId];
            if (!priceMatch(top)) break;

            int tradedQty = std::min(incoming.quantity, top.quantity);
            double tradePrice = top.price;

            MatchResult match = {
                    incoming.userId, top.userId,
                    tradePrice, tradedQty,
                    incoming.orderId, top.orderId
            };
            results.push_back(match);

            incoming.quantity -= tradedQty;
            top.quantity -= tradedQty;

            sellOrders.pop();
            if (top.quantity > 0) {
                orderLookup[top.orderId] = top;
                sellOrders.push(top);
            } else {
                orderLookup.erase(top.orderId);
            }
        }
    } else {
        while (!buyOrders.empty() && incoming.quantity > 0) {
            OrderEntry top = buyOrders.top();

            if (orderLookup.find(top.orderId) == orderLookup.end()) {
                buyOrders.pop();
                continue;
            }

            top = orderLookup[top.orderId];
            if (!priceMatch(top)) break;

            int tradedQty = std::min(incoming.quantity, top.quantity);
            double tradePrice = top.price;

            MatchResult match = {
                    top.userId, incoming.userId,
                    tradePrice, tradedQty,
                    top.orderId, incoming.orderId
            };
            results.push_back(match);

            incoming.quantity -= tradedQty;
            top.quantity -= tradedQty;

            buyOrders.pop();
            if (top.quantity > 0) {
                orderLookup[top.orderId] = top;
                buyOrders.push(top);
            } else {
                orderLookup.erase(top.orderId);
            }
        }
    }

    if (incoming.quantity > 0) {
        orderLookup[incoming.orderId] = incoming;
        if (incoming.type == OrderType::BUY) {
            buyOrders.push(incoming);
        } else {
            sellOrders.push(incoming);
        }
    } else {
        orderLookup.erase(incoming.orderId);
    }

    return results;
}

void OrderBook::clear() {
    while (!buyOrders.empty()) buyOrders.pop();
    while (!sellOrders.empty()) sellOrders.pop();
    orderLookup.clear();
}

const std::unordered_map<std::string, OrderEntry>& OrderBook::getActiveOrders() const {
    return orderLookup;
}
