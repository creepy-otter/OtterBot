#include <glog/logging.h>

#include <iomanip>
#include <iostream>

#include "TwsApiDefs.h"
#include "data/CSVData.h"
#include "data/IBKRHisData.h"
#include "event/DataUpdateEvent.h"
#include "event/EventDispatcher.h"
#include "execution/BacktestHandler.h"
#include "portfolio/Portfolio.h"
#include "strategy/TestStrategy.h"

/// Faster: Check spelling of parameter at compile time instead of runtime.
// #include "TwsApiDefs.h"

using namespace otterbot;
using namespace TwsApi;

class YourEWrapper : public EWrapperL0 {
 public:
  bool m_Done, m_ErrorForRequest;
  bool notDone(void) { return !(m_Done); }

  /// Easier: The EReader calls all methods automatically(optional)
  YourEWrapper(bool runEReader = true) : EWrapperL0(runEReader) {
    m_Done = false;
    m_ErrorForRequest = false;
  }

  /// Methods winError & error print the errors reported by IB TWS
  virtual void winError(const IBString& str, int lastError) {
    fprintf(stderr, "WinError: %d = %s\n", lastError, (const char*)str);
    m_ErrorForRequest = true;
  }

  virtual void error(const int id, const int errorCode,
                     const IBString errorString) {
    fprintf(stderr, "Error for id=%d: %d = %s\n", id, errorCode,
            (const char*)errorString);
    m_ErrorForRequest =
        (id > 0);  // id == -1 are 'system' messages, not for user requests
  }

  /// Safer: uncatched exceptions are catched before they reach the IB library
  /// code.
  ///        The Id is tickerId, orderId, or reqId, or -1 when no id known
  virtual void OnCatch(const char* MethodName, const long Id) {
    fprintf(stderr, "*** Catch in EWrapper::%s( Id=%ld, ...) \n", MethodName,
            Id);
  }

  /// Faster: Implement only the method for the task.
  ///  => TwsApiC++  provides an empty implementation for each EWrapper method.
  virtual void historicalData(TickerId reqId, const IBString& date, double open,
                              double high, double low, double close, int volume,
                              int barCount, double WAP, int hasGaps) {
    /// Easier: EWrapperL0 provides an extra method to check all data was
    /// retrieved
    if (IsEndOfHistoricalData(date)) {
      m_Done = true;
      return;
    }

    fprintf(stdout, "%3ld, %10s, %5.3f, %5.3f, %5.3f, %5.3f, %7d\n", reqId,
            (const char*)date, open, high, low, close, volume);
  }
};

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);

  EventDispatcher dispatcher;
  CSVData csvData(dispatcher, "AAPL", "../data/aapl.csv");
  IBKRHisData hisData(dispatcher, "AAPL");
  Portfolio portfolio(dispatcher, 1);
  TestStrategy strategy(dispatcher, "AAPL", 0.01);
  BacktestHandler backtest(dispatcher, portfolio);

  YourEWrapper wrapper(false);

  while (!csvData.isEnd())
    ;
  std::cout << "P&L: " << portfolio.getPnL() << std::endl;

  EClientL0* EC = EClientL0::New(&wrapper);
  Order order;
  order.action = *OrderAction::BUY;
  order.orderType = *TwsApi::OrderType::LMT;
  order.totalQuantity = 5;
  order.lmtPrice = 10;
  order.discretionaryAmt = 1;
  /// Easier: All structures defined by IB are immediately available.
  /// Faster: Use of TwsApiDefs.h codes check typos at compile time
  Contract C;
  C.symbol = "MAAFDI";
  C.secType = *SecType::STK;  // instead of "STK"
  C.currency = "USD";
  C.exchange = *Exchange::IB_SMART;     // instead of "SMART"
  C.primaryExchange = *Exchange::AMEX;  // instead of "AMEX"

  Contract C2;
  C2.symbol = "AAPL";
  C2.secType = *SecType::STK;  // instead of "STK"
  C2.currency = "USD";
  C2.exchange = *Exchange::IB_SMART;     // instead of "SMART"
  C2.primaryExchange = *Exchange::AMEX;  // instead of "AMEX"

  /// Easier: Use of TwsApiDefs.h makes code self explanatory,
  ///         i.e. UseRTH::OnlyRegularTradingData instead of true or 1.
  if (EC->eConnect("", 7496, 100)) {
    // EC->reqHistoricalData(
    //     21, C2, EndDateTime(2013, 02, 20)  // 20130220 00:00:00
    //     ,
    //     DurationStr(1, *DurationHorizon::Months)  // instead of "1 M"
    //     ,
    //     *BarSizeSetting::_1_day  // instead of "1 day"
    //     ,
    //     *WhatToShow::TRADES  // instead of "TRADES"
    //     ,
    //     UseRTH::OnlyRegularTradingData  // instead of 1
    //     ,
    //     FormatDate::AsDate  // instead of 1
    // );
    EC->reqHistoricalData(
        20, C2, EndDateTime(2013, 02, 20)  // 20130220 00:00:00
        ,
        DurationStr(1, *DurationHorizon::Months)  // instead of "1 M"
        ,
        *BarSizeSetting::_10_mins  // instead of "1 day"
        ,
        *WhatToShow::TRADES  // instead of "TRADES"
        ,
        UseRTH::OnlyRegularTradingData  // instead of 1
        ,
        FormatDate::AsDate  // instead of 1
    );

    /// Easier: Call checkMessages() in a loop. No need to wait between two
    /// calls.
    // EC->placeOrder(1, C, order);
    while (wrapper.notDone()) {
      EC->checkMessages();
    }
  }

  EC->eDisconnect();
  delete EC;
}