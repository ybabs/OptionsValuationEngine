#pragma once

#include "IEventListener.hpp"
#include "events/IEvent.hpp"

#include <vector>
#include <memory>

class EventManager
{
public:
    void subscribe(std::shared_ptr<IEventListener> listener);
    void unsubscribe(std::shared_ptr<IEventListener> listener);
    void notify(const IEvent& event);

private:
    std::vector<std::shared_ptr<IEventListener>> observers_;

};