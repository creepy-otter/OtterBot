#include <iomanip>
#include <iostream>

#include "data/CSVData.h"
#include "event/DataUpdateEvent.h"
#include "event/EventDispatcher.h"
#include "strategy/TestStrategy.h"

using namespace otterbot;

int main() {
  EventDispatcher dispatcher;
  CSVData csvData(dispatcher, "AAPL", "../OtterBot/data/aapl.csv");
  TestStrategy strategy(dispatcher);

  auto hid = dispatcher.register_handler(
      EventType::DataUpdate, [&strategy](const Event& event) {
        strategy.onDataUpdate(dynamic_cast<const DataUpdateEvent&>(event));
      });  // EventHandler ID
  return 0;
}