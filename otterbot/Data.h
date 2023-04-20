#pragma once

#include <chrono>
#include <string>

#include "date.h"

namespace otterbot {

struct Data {
  date::sys_seconds timestamp;
  double last;
  int volume;
};

struct TickData : Data{
  double bid;
  int bidVolume;
  double ask;
  int askVolume;
};

struct BarData : Data{
  double open;
  double high;
  double low;
};

}; // namespace otterbot