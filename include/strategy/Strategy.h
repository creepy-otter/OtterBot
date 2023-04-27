#pragma once

#include "event/DataUpdateEvent.h"
#include "event/EventDispatcher.h"
#include "event/OrderFillEvent.h"

namespace otterbot {

class Strategy {
 public:
  explicit Strategy(EventDispatcher& dispatcher) : dispatcher_(dispatcher) {}

  virtual void onDataUpdate(const DataUpdateEvent& event) = 0;
  virtual void onOrderFill(const OrderFillEvent& event) = 0;

 protected:
  EventDispatcher& dispatcher_;
};

}  // namespace otterbot
