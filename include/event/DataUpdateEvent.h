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
  std::string get_symbol() const { return symbol_; }
  double get_price() const { return price_; }
  int get_volume() const { return volume_; }
  date::sys_seconds get_time() const { return timestamp_; }

 private:
  std::string symbol_;
  double price_;
  int volume_;
  date::sys_seconds timestamp_;
};

}  // namespace otterbot
