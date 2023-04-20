#pragma once

#include <string>
#include <vector>
#include "DataFeed.h"
#include "csv.hpp"

namespace otterbot {

namespace YF {
  const std::string Timestamp = "Datetime";
  const std::string Last = "Close";
  const std::string Open = "Open";
  const std::string High = "High";
  const std::string Low = "Low";
  const std::string Volume = "Volume";
}

// Read bar data from yfinance.
class CSVBarDataFeed : public DataFeed<BarData> {
public:
  CSVBarDataFeed(const std::string& symbol, const std::string& path);
  virtual void subscribe();
  virtual BarData getData();
  void addData(const BarData& data);
protected:
  const std::string path_;
};

} // namespace otterbot
