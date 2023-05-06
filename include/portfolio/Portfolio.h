#pragma once

#include <atomic>
#include <shared_mutex>
#include <unordered_map>

#include "event/EventDispatcher.h"
#include "event/OrderEvent.h"
#include "event/OrderFillEvent.h"
#include "event/OrderRejectEvent.h"

namespace otterbot {

class Portfolio {
 public:
  Portfolio(EventDispatcher& dispatcher, double initial_cash);

  void onOrderFill(const OrderFillEvent& event);
  void onOrder(const OrderEvent& event);
  void onOrderReject(const OrderRejectEvent& event);

  double getCash() const;
  double getPnL() const;
  double getPower() const;
  int getPosition(const std::string& symbol) const;

 private:
  EventDispatcher& dispatcher_;
  double cash_;
  double pnl_;
  double power_;
  std::unordered_map<std::string, int> positions_;
  mutable std::shared_mutex mtx_;
};
}  // namespace otterbot