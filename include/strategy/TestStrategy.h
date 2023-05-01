#pragma once

#include "strategy/Strategy.h"

namespace otterbot {

class TestStrategy : public Strategy {
 public:
  explicit TestStrategy(EventDispatcher& dispatcher, const std::string& symbol,
                        double distance);
  void onDataUpdate(const DataUpdateEvent& event) override;
  void onOrderFill(const OrderFillEvent& event) override;

 private:
  OrderId placeBuyOrder(double price);
  OrderId placeSellOrder(double price);
  std::string symbol_;
  double distance_;
  int position_;
};

}  // namespace otterbot