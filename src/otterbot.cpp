#include <iomanip>
#include <iostream>

#include "data/CSVData.h"
#include "event/DataUpdateEvent.h"
#include "event/EventDispatcher.h"

using namespace otterbot;

int main() {
  EventDispatcher dispatcher;
  CSVData csvData(dispatcher, "AAPL", "../OtterBot/data/aapl.csv");
  dispatcher.register_handler(EventType::DataUpdate, [](const Event& event) {
    const DataUpdateEvent dEvent = dynamic_cast<const DataUpdateEvent&>(event);
    std::time_t t = std::chrono::system_clock::to_time_t(dEvent.get_time());
    std::cout << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S") << " "
              << dEvent.get_price() << std::endl;
  });
  return 0;
}