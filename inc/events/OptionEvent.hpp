#include "IEvent.hpp"
#include <string>

class OptionEvent : public IEvent
{
public:
 OptionEvent(EventType type, const std::string& message): eventType_(type), message_(message)
 {

 }

 EventType type() const override
 {
    return eventType_;
 }

 std::string message() const override
 {
    return message_;
 }



 private:
  EventType eventType_;
  std::string message_;

};