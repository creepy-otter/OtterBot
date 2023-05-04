#pragma once

#include "event/DataUpdateEvent.h"
#include "event/EventDispatcher.h"
#include "event/OrderEvent.h"
#include "event/OrderFillEvent.h"
#include "event/OrderRejectEvent.h"

namespace otterbot {

class Strategy {
 public:
  explicit Strategy(EventDispatcher& dispatcher) : dispatcher_(dispatcher) {
    dispatcher_.register_handler(
        EventType::DataUpdate, [this](const Event& event) {
          this->onDataUpdate(dynamic_cast<const DataUpdateEvent&>(event));
        });
    dispatcher_.register_handler(
        EventType::OrderFill, [this](const Event& event) {
          this->onOrderFill(dynamic_cast<const OrderFillEvent&>(event));
        });
    dispatcher_.register_handler(
        EventType::OrderReject, [this](const Event& event) {
          this->onOrderReject(dynamic_cast<const OrderRejectEvent&>(event));
        });
  }

  virtual void onDataUpdate(const DataUpdateEvent& event) = 0;
  virtual void onOrderFill(const OrderFillEvent& event) = 0;
  virtual void onOrderReject(const OrderRejectEvent& event) = 0;

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
