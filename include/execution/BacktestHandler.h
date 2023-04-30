#pragma once

#include <unordered_map>
#include <vector>

#include "event/DataUpdateEvent.h"
#include "event/EventDispatcher.h"
#include "event/EventType.h"
#include "event/OrderEvent.h"
#include "execution/ExecutionHandler.h"

namespace otterbot {
class BacktestHandler : public ExecutionHandler {
 public:
  explicit BacktestHandler(EventDispatcher& dispatcher);
  void onOrder(const OrderEvent& order) override;
  void onDataUpdate(const DataUpdateEvent& event);
  void onOrderFill() override;

 protected:
  std::unordered_map<std::string, std::vector<OrderEvent>> order_map_;
};
}  // namespace otterbot
