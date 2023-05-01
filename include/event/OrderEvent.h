#pragma once

#include <string>

#include "Event.h"

namespace otterbot {

class OrderEvent : public Event {
 public:
  OrderEvent(const std::string& symbol, OrderType order_type,
             OrderSide order_side, double price, double quantity, OrderId id)
      : Event(EventType::Order),
        symbol_(symbol),
        order_type_(order_type),
        order_side_(order_side),
        price_(price),
        quantity_(quantity),
        id_(id) {}

  const std::string& getSymbol() const { return symbol_; }
  OrderType getOrderType() const { return order_type_; }
  OrderSide getOrderSide() const { return order_side_; }
  double getPrice() const { return price_; }
  double getQuantity() const { return quantity_; }
  OrderId getOrderId() const { return id_; }

 private:
  std::string symbol_;
  OrderType order_type_;
  OrderSide order_side_;
  double price_;
  double quantity_;
  OrderId id_;
};

}  // namespace otterbot