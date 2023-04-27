#pragma once

#include "strategy/Strategy.h"

namespace otterbot {

class TestStrategy : public Strategy {
 public:
  explicit TestStrategy(EventDispatcher& dispatcher);
  void onDataUpdate(const DataUpdateEvent& event) override;
  void onOrderFill(const OrderFillEvent& event) override;
};

}  // namespace otterbot