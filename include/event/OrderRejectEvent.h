#pragma once

#include <string>

#include "event/Event.h"

namespace otterbot {

class OrderRejectEvent : public Event {
 public:
  OrderRejectEvent(OrderId id, const std::string& symbol, OrderType order_tpye,
                   OrderSide order_side, double price, double quantity,
                   const std::string& message)
      : Event(EventType::OrderReject),
        id_(id),
        symbol_(symbol),
        order_type_(order_tpye),
        order_side_(order_side),
        price_(price),
        quantity_(quantity),
        message_(message) {}

  const std::string& getSymbol() const { return symbol_; }
  OrderType getOrderType() const { return order_type_; }
  OrderSide getOrderSide() const { return order_side_; }
  double getPrice() const { return price_; }
  double getQuantity() const { return quantity_; }
  OrderId getOrderId() const { return id_; }
  const std::string& getMessage() const { return message_; }

 private:
  OrderId id_;
  const std::string symbol_;
  OrderType order_type_;
  OrderSide order_side_;
  double price_;
  double quantity_;
  const std::string message_;
};

}  // namespace otterbot
