#pragma once

#include <string>

#include "event/Event.h"

namespace otterbot {

class OrderRejectEvent : public Event {
 public:
  OrderRejectEvent(OrderId id, const std::string& message)
      : Event(EventType::OrderReject), id_(id), message_(message) {}

 private:
  OrderId id_;
  const std::string message_;
};

}  // namespace otterbot
