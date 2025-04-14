#include "IEvent.hpp"

class IEventListener 
{
public:
    virtual ~IEventListener() = default;

    //Should have used QT and just used the Signal/Slot mechanism to be honest. 
    virtual void onEvent(const IEvent& event) = 0;

};