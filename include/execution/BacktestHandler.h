#pragma once

#include <unordered_map>
#include <vector>

#include "event/DataUpdateEvent.h"
#include "event/EventDispatcher.h"
#include "event/EventType.h"
#include "event/OrderEvent.h"
#include "execution/ExecutionHandler.h"
#include "portfolio/Portfolio.h"

namespace otterbot {
class BacktestHandler : public ExecutionHandler {
 public:
  explicit BacktestHandler(EventDispatcher& dispatcher, Portfolio& portfolio_);
  void onOrder(const OrderEvent& order) override;
  void onDataUpdate(const DataUpdateEvent& event);
  void onOrderFill() override;

 protected:
  std::unordered_map<std::string, std::vector<OrderEvent>> order_map_;
  Portfolio& portfolio_;
};
}  // namespace otterbot
