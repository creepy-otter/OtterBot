#pragma once

#include "event/EventDispatcher.h"
#include "event/OrderEvent.h"
namespace otterbot {

class ExecutionHandler {
 public:
  ExecutionHandler(EventDispatcher& dispatcher) : dispatcher_(dispatcher) {}
  virtual void onOrder(const OrderEvent& order) = 0;
  virtual void onOrderFill() = 0;

 protected:
  EventDispatcher& dispatcher_;
};

}  // namespace otterbot