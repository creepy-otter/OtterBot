#pragma once

#include <unordered_map>

#include "index/MA.h"
#include "strategy/Strategy.h"

namespace otterbot {
/**
 * A Simple moving average cross strategy that applies to several instruments.
 * The strategy will place a buy order when the fast moving average crosses
 * above the slow moving average, and place a sell order when the fast moving
 * average crosses below the slow moving average.
 */
class SMACrossStrategy : public Strategy {
 public:
  explicit SMACrossStrategy(EventDispatcher& dispatcher, int fast, int slow);
  void onDataUpdate(const DataUpdateEvent& event) override;
  void onOrderFill(const OrderFillEvent& event) override;
  void onOrderReject(const OrderRejectEvent& event) override;

 private:
  OrderId placeBuyOrder(const std::string& symbol, double price);
  OrderId placeSellOrder(const std::string& symbol, double price);
  int fast_;
  int slow_;
  std::unordered_map<std::string, int> position_;
  std::unordered_map<std::string, MA> fastMA_;
  std::unordered_map<std::string, MA> slowMA_;
};
}  // namespace otterbot