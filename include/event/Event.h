#pragma once

#include "EventType.h"

namespace otterbot {

class Event {
 public:
  Event(EventType type) : type(type) {}
  EventType getType() const { return type; }
  virtual ~Event() = default;

 private:
  EventType type;
};

}  // namespace otterbot
