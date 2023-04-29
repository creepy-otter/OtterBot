#pragma once

#include <unordered_map>
#include <vector>

#include "event/EventDispatcher.h"
#include "event/EventType.h"
#include "event/OrderEvent.h"
#include "event/OrderFillEvent.h"
#include "execution/ExecutionHandler.h"

namespace otterbot {
class BacktestHandler : public ExecutionHandler {
 public:
  explicit BacktestHandler(EventDispatcher& dispatcher)
      : ExecutionHandler(dispatcher) {}
  void onOrder(const OrderEvent& order) override {
    if (order.getOrderType() == OrderType::LIMIT) {
      order_map_[order.getSymbol()].push_back(order);
    } else {
      OrderFillEvent fill_event(order.getSymbol(), order.getOrderSide(),
                                order.getPrice(), order.getQuantity());
    }
  }
  void onDataUpdate(const DataUpdateEvent& event) {
    const std::string& symbol = event.getSymbol();
    double price = event.getPrice();

    if (order_map_.count(symbol) > 0) {
      auto& orders = order_map_[symbol];
      for (size_t i = 0; i < orders.size();) {
        OrderEvent& order = orders[i];
        bool filled = false;
        if (order.getOrderType() == OrderType::LIMIT) {
          if (order.getOrderSide() == OrderSide::BUY &&
              price <= order.getPrice())
            filled = true;
          else if (order.getOrderSide() == OrderSide::SELL &&
                   price >= order.getPrice())
            filled = true;
        } else if (order.getOrderType() == OrderType::STOP) {
          if (order.getOrderSide() == OrderSide::BUY &&
              price >= order.getPrice())
            filled = true;
          else if (order.getOrderSide() == OrderSide::SELL &&
                   price <= order.getPrice())
            filled = true;
        }

        if (filled) {
          OrderFillEvent fill_event(symbol, order.getOrderSide(), price,
                                    order.getQuantity());
          dispatcher_.dispatch(fill_event);

          std::swap(order, orders.back());
          orders.pop_back();
        } else {
          ++i;
        }
      }
    }
  }
  void onOrderFill() override {}

 protected:
  std::unordered_map<std::string, std::vector<OrderEvent>> order_map_;
};
}  // namespace otterbot
