#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

#include "Data.h"

namespace otterbot{

template <typename DataType>
class DataFeed {
public:
  DataFeed(const std::string& symbol): symbol_(symbol) {}
  virtual void subscribe() = 0;
  virtual DataType getData() = 0;
  virtual void addData(const DataType& data) = 0;
  virtual bool empty() {return dataq_.empty();}
protected:
  std::queue<DataType> dataq_;
  std::mutex mutex_;
  std::condition_variable cv_;
  const std::string symbol_;
};

} // namespace otterbot