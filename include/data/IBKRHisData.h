#pragma once

#include "TwsApiDefs.h"
#include "event/EventDispatcher.h"
#include "lib/date.h"

namespace otterbot {

class IBKRHisData : public EWrapperL0 {
 public:
  explicit IBKRHisData(EventDispatcher& dispatcher, const std::string& symbol);
  virtual void historicalData(TickerId reqId, const IBString& date, double open,
                              double high, double low, double close, int volume,
                              int barCount, double WAP, int hasGaps) override;
  void subscribe();
  bool isEnd() const;
  void on_data_received(const std::string& symbol, double price, int volume,
                        date::sys_seconds timestamp);

  virtual void error(const int id, const int errorCode,
                     const IBString errorString) override;

  virtual ~IBKRHisData();

 protected:
  void connect();
  EventDispatcher& dispatcher_;
  std::string symbol_;
  std::unique_ptr<EClientL0> client_;
  bool isEnd_ = false;
};

}  // namespace otterbot