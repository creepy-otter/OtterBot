#include "data/IBKRHisData.h"

#include <glog/logging.h>

#include "event/DataUpdateEvent.h"
#include "ibkr/IBKRUtils.h"
#include "lib/csv.hpp"
#include "lib/date.h"

namespace otterbot {

using namespace csv;
using namespace date;
using namespace TwsApi;

IBKRHisData::IBKRHisData(EventDispatcher& dispatcher, const std::string& symbol,
                         IBString endTime, IBString duration,
                         TwsApi::BarSizeSetting barSize)
    : EWrapperL0(true),
      dispatcher_(dispatcher),
      symbol_(symbol),
      endTime_(endTime),
      duration_(duration),
      barSize_(barSize) {
  connect();
}

void IBKRHisData::historicalData(TickerId reqId, const IBString& dt,
                                 double open, double high, double low,
                                 double close, int volume, int barCount,
                                 double WAP, int hasGaps) {
  int timestamp = std::stoi(dt);
  sys_seconds ts{std::chrono::seconds{timestamp}};
  on_data_received(symbol_, close, volume, ts);
  if (IsEndOfHistoricalData(dt)) {
    isEnd_ = true;
  }
}

void IBKRHisData::subscribe() {
  Contract C;
  C.symbol = symbol_;
  C.secType = *SecType::STK;
  C.currency = "USD";
  C.exchange = *Exchange::IB_SMART;
  C.primaryExchange = *Exchange::AMEX;
  client_->reqHistoricalData(getNextTickerId(), C, endTime_, duration_,
                             TwsApi::operator*(barSize_), *WhatToShow::TRADES,
                             UseRTH::OnlyRegularTradingData,
                             FormatDate::AsSecondsSince);
}

bool IBKRHisData::isEnd() const { return isEnd_; }

void IBKRHisData::on_data_received(const std::string& symbol, double price,
                                   int volume, date::sys_seconds timestamp) {
  DataUpdateEvent e(symbol, price, volume, timestamp);
  dispatcher_.dispatch(e);
}

void IBKRHisData::OnCatch(const char* MethodName, const long Id) {
  std::cout << "catch: " << MethodName << ", id: " << Id << std::endl;
  isEnd_ = true;
}

void IBKRHisData::error(const int id, const int errorCode,
                        const IBString errorString) {
  std::cout << "err id: " << id << ", code" << errorCode
            << ", msg: " << errorString << std::endl;
}

void IBKRHisData::connect() {
  client_ = std::unique_ptr<EClientL0>(EClientL0::New(this));
  if (!client_->eConnect("", 7496, 104)) {
    throw std::runtime_error("Failed connect to TWS");
  }
}

IBKRHisData::~IBKRHisData() {
  if (client_) {
    client_->eDisconnect();
  }
}

}  // namespace otterbot
