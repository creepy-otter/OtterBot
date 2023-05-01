#pragma once

#include "event/DataUpdateEvent.h"
#include "event/EventDispatcher.h"
#include "event/OrderEvent.h"
#include "event/OrderFillEvent.h"

namespace otterbot {

class Strategy {
 public:
  explicit Strategy(EventDispatcher& dispatcher) : dispatcher_(dispatcher) {}

  virtual void onDataUpdate(const DataUpdateEvent& event) = 0;
  virtual void onOrderFill(const OrderFillEvent& event) = 0;

 protected:
  OrderId placeOrder(const std::string& symbol, OrderType type, OrderSide side,
                     double price, int quantity) {
    OrderEvent order(symbol, type, side, price, quantity, nextId);
    dispatcher_.dispatch(order);
    return nextId++;
  }
  EventDispatcher& dispatcher_;
  OrderId nextId{0};
};

}  // namespace otterbot
