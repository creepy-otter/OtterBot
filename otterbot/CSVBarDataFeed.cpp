#include <iomanip>
#include <sstream>
#include "CSVBarDataFeed.h"

namespace otterbot {

using namespace csv;
using namespace date;

CSVBarDataFeed::CSVBarDataFeed(const std::string& symbol, const std::string& path)
: DataFeed(symbol), path_(path) {}

void CSVBarDataFeed::subscribe() {
  CSVReader reader(path_);
  BarData bar;
  for (CSVRow& row : reader) {
    std::istringstream in(row[YF::Timestamp].get<std::string>());
    sys_seconds tp;
    in >> parse("%Y-%m-%d %H:%M:%S%z", tp);
    bar.timestamp = tp;
    bar.last = row[YF::Last].get<double>();
    bar.volume = row[YF::Volume].get<int>();
    bar.open = row[YF::Open].get<double>();
    bar.high = row[YF::High].get<double>();
    bar.low = row[YF::Low].get<double>();
    addData(bar);
  }
}

BarData CSVBarDataFeed::getData() {
  std::unique_lock<std::mutex> lock(mutex_);
  while (dataq_.empty()) {
    cv_.wait(lock);
  }
  BarData bar = dataq_.front();
  dataq_.pop();
  return bar;
}

void CSVBarDataFeed::addData(const BarData& data) {
  std::unique_lock<std::mutex> lock(mutex_);
  dataq_.push(data);
  cv_.notify_one();
}

} // namespcae otterbot