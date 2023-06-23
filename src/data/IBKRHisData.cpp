#include "data/IBKRHisData.h"

#include "event/DataUpdateEvent.h"
#include "ibkr/IBKRUtils.h"
#include "lib/csv.hpp"

namespace otterbot {

using namespace csv;
using namespace date;
using namespace TwsApi;

IBKRHisData::IBKRHisData(EventDispatcher& dispatcher, const std::string& symbol)
    : EWrapperL0(true), dispatcher_(dispatcher), symbol_(symbol) {
  connect();
}

void IBKRHisData::historicalData(TickerId reqId, const IBString& date,
                                 double open, double high, double low,
                                 double close, int volume, int barCount,
                                 double WAP, int hasGaps) {
  std::istringstream in(date);
  sys_seconds ts;
  in >> parse("%Y%m%d %H:%M:%S", ts);
  on_data_received(symbol_, close, volume, ts);
  if (IsEndOfHistoricalData(date)) {
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
  client_->reqHistoricalData(getNextTickerId(), C, EndDateTime(2013, 02, 20),
                             DurationStr(1, *DurationHorizon::Months),
                             *BarSizeSetting::_10_mins, *WhatToShow::TRADES,
                             UseRTH::OnlyRegularTradingData,
                             FormatDate::AsSecondsSince);
}

bool IBKRHisData::isEnd() const { return isEnd_; }

void IBKRHisData::on_data_received(const std::string& symbol, double price,
                                   int volume, date::sys_seconds timestamp) {
  DataUpdateEvent e(symbol, price, volume, timestamp);
  dispatcher_.dispatch(e);
}

void IBKRHisData::error(const int id, const int errorCode,
                        const IBString errorString) {
  std::cout << "err id: " << id << ", code" << errorCode
            << ", msg: " << errorString << std::endl;
}

void IBKRHisData::connect() {
  client_ = std::unique_ptr<EClientL0>(EClientL0::New(this));
  if (!client_->eConnect("", 7496, 101)) {
    throw std::runtime_error("Failed connect to TWS");
  }
}

IBKRHisData::~IBKRHisData() {
  if (client_) {
    client_->eDisconnect();
  }
}

}  // namespace otterbot
