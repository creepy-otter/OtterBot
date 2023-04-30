#pragma once

#include <atomic>
#include <shared_mutex>
#include <unordered_map>

#include "event/EventDispatcher.h"
#include "event/OrderFillEvent.h"

namespace otterbot {

class Portfolio {
 public:
  Portfolio(EventDispatcher& dispatcher, double initial_cash);

  void onOrderFill(const OrderFillEvent& event);

  double getCash() const;
  double getPnL() const;
  int getPosition(const std::string& symbol) const;

 private:
  double cash_;
  double pnl_;
  std::unordered_map<std::string, int> positions_;
  EventDispatcher& dispatcher_;
  mutable std::shared_mutex mtx_;
};
}  // namespace otterbot