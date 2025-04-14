#pragma once

#include <string>

// Going to have to maintain this if I add more 
// derivative types here
enum class EventType 
{
    OptionBuilt,
    OptionPriced,
    OptionParamaterUpdated
};

class IEvent
{
public:
    virtual ~IEvent() = default;

    // Returns type of Event
    virtual EventType type() const = 0;

    // Returns a message to accompany the event
    //TODO: Need to think of something better to go with this
    virtual std::string message() const = 0;

};