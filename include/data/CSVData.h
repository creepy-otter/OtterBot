#pragma once

#include <atomic>
#include <string>
#include <thread>

#include "event/EventDispatcher.h"
#include "lib/date.h"

namespace otterbot {

namespace YF {
const std::string Timestamp = "Datetime";
const std::string Last = "Close";
const std::string Open = "Open";
const std::string High = "High";
const std::string Low = "Low";
const std::string Volume = "Volume";
}  // namespace YF

class CSVData {
 public:
  explicit CSVData(EventDispatcher& dispatcher, const std::string& symbol,
                   const std::string& path);
  void on_data_received(const std::string& symbol, double price, int volume,
                        date::sys_seconds timestamp);
  void subscribe();

  bool isEnd() const;

  ~CSVData();

 protected:
  EventDispatcher& dispatcher_;
  std::string symbol_;
  std::string path_;
  std::thread subscribe_thread_;
  std::atomic<bool> eof_{false};
};
}  // namespace otterbot
