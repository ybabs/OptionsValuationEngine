#include "events/EventManager.hpp"
#include "events/IEvent.hpp"
#include <algorithm>
#include <memory>


void EventManager::subscribe(std::shared_ptr<IEventListener> listener)
{
    if(listener)
    {
        observers_.push_back(listener);
    }
}

void EventManager::unsubscribe(std::shared_ptr<IEventListener> listener)
{
    observers_.erase(
        std::remove_if(observers_.begin(), observers_.end(),
        
        [&listener](const std::shared_ptr<IEventListener>& obs)
        {
            return obs == listener;
        }),
        observers_.end());
}

void EventManager::notify(const IEvent& event)
{
    for(auto observer: observers_)
    {
        observer->onEvent(event);
    }
}