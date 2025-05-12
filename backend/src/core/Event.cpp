#include "core/Event.hpp"

Event::Event(std::time_t timestamp)
        : timestamp(timestamp) {}

std::time_t Event::getTimestamp() const {
    return timestamp;
}
