#include <glog/logging.h>

#include <iomanip>
#include <iostream>

#include "TwsApiDefs.h"
#include "data/IBKRHisData.h"
#include "event/EventDispatcher.h"
#include "execution/BacktestHandler.h"
#include "portfolio/Portfolio.h"
#include "strategy/SMACrossStrategy.h"

using namespace otterbot;
using namespace TwsApi;

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  FLAGS_log_dir = "/home/liuhao/workplace/logs";
  EventDispatcher dispatcher;
  IBKRHisData hisData(dispatcher, "AAPL", EndDateTime(2023, 02, 20),
                      DurationStr(1, *DurationHorizon::Days),
                      BarSizeSetting::_1_min);
  Portfolio portfolio(dispatcher, 1000000);
  SMACrossStrategy(dispatcher, 5, 50);
  BacktestHandler backtest(dispatcher, portfolio);
  hisData.subscribe();
  std::cout << "checking end" << std::endl;
  while (!hisData.isEnd())
    ;
  LOG(INFO) << "PnL%: " << portfolio.getPnL() / 1000000;
  std::cout << "PnL%: " << portfolio.getPnL() / 1000000 << std::endl;
  return 0;
}