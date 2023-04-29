#include "data/CSVData.h"

#include "event/DataUpdateEvent.h"
#include "lib/csv.hpp"

namespace otterbot {

using namespace csv;
using namespace date;

CSVData::CSVData(EventDispatcher& dispatcher, const std::string& symbol,
                 const std::string& path)
    : dispatcher_(dispatcher), symbol_(symbol), path_(path) {
  subscribe_thread_ = std::thread(&CSVData::subscribe, this);
}

void CSVData::subscribe() {
  CSVReader reader(path_);
  for (CSVRow& row : reader) {
    std::istringstream in(row[YF::Timestamp].get<std::string>());
    sys_seconds ts;
    in >> parse("%Y-%m-%d %H:%M:%S%z", ts);
    on_data_received(symbol_, row[YF::Last].get<double>(),
                     row[YF::Volume].get<int>(), ts);
  }
  eof_.store(true);
}

void CSVData::on_data_received(const std::string& symbol, double price,
                               int volume, sys_seconds timestamp) {
  DataUpdateEvent e(symbol, price, volume, timestamp);
  dispatcher_.dispatch(e);
}

bool CSVData::isEnd() const { return eof_.load(); }

CSVData::~CSVData() {
  if (subscribe_thread_.joinable()) {
    subscribe_thread_.join();
  }
}

}  // namespace otterbot