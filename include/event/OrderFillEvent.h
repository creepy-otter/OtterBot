#pragma once

#include <string>

#include "Event.h"

namespace otterbot {

class OrderFillEvent : public Event {
 public:
  OrderFillEvent(const std::string& symbol, OrderSide side, double fill_price,
                 double fill_quantity, OrderId id)
      : Event(EventType::OrderFill),
        symbol_(symbol),
        side_(side),
        fill_price_(fill_price),
        fill_quantity_(fill_quantity),
        id_(id) {}

  const std::string& getSymbol() const { return symbol_; }

  const OrderSide& getOrderSide() const { return side_; }

  double getFillPrice() const { return fill_price_; }

  double getFillQuantity() const { return fill_quantity_; }

  OrderId getOrderId() const { return id_; }

 private:
  std::string symbol_;
  OrderSide side_;
  double fill_price_;
  double fill_quantity_;
  OrderId id_;
};

}  // namespace otterbot
