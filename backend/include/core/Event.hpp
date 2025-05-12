#ifndef EVENT_HPP
#define EVENT_HPP

#include <string>
#include <ctime>

class Event {
public:
    Event(std::time_t timestamp);
    virtual ~Event() = default;

    virtual std::string getType() const = 0;
    std::time_t getTimestamp() const;

    virtual std::string serialize() const = 0;

protected:
    std::time_t timestamp;
};

#endif // EVENT_HPP
