#ifndef EVENT_STORE_HPP
#define EVENT_STORE_HPP

#include <vector>
#include <memory>
#include <string>
#include "Event.hpp"

class EventStore {
public:
    explicit EventStore(const std::string& filename);

    void append(std::shared_ptr<Event> event);
    std::vector<std::shared_ptr<Event>> getAllEvents();
    std::vector<std::shared_ptr<Event>> getEventsUpTo(std::time_t timestamp);
    void clear();

private:
    std::vector<std::shared_ptr<Event>> events;
    std::string logFile;

    void loadFromFile();
    void appendToFile(const std::shared_ptr<Event>& event);
};

#endif // EVENT_STORE_HPP
