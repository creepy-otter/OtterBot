#pragma once

#include <string>

#include "event/Event.h"
#include "lib/date.h"

namespace otterbot {
class DataUpdateEvent : public Event {
 public:
  DataUpdateEvent(const std::string& symbol, double price, int volume,
                  date::sys_seconds timestamp)
      : Event(EventType::DataUpdate),
        symbol_(symbol),
        price_(price),
        volume_(volume),
        timestamp_(timestamp) {}
  std::string getSymbol() const { return symbol_; }
  double getPrice() const { return price_; }
  int getVolume() const { return volume_; }
  date::sys_seconds getTime() const { return timestamp_; }

 private:
  std::string symbol_;
  double price_;
  int volume_;
  date::sys_seconds timestamp_;
};

}  // namespace otterbot
