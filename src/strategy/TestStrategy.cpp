#include "strategy/TestStrategy.h"

#include <iomanip>
#include <iostream>

namespace otterbot {

TestStrategy::TestStrategy(EventDispatcher& dispatcher)
    : Strategy(dispatcher) {}

void TestStrategy::onDataUpdate(const DataUpdateEvent& event) {
  std::time_t t = std::chrono::system_clock::to_time_t(event.get_time());
  std::cout << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S") << " "
            << event.get_price() << std::endl;
}

void TestStrategy::onOrderFill(const OrderFillEvent& event) {}

}  // namespace otterbot