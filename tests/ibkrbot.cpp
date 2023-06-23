#include <glog/logging.h>

#include <iomanip>
#include <iostream>

#include "TwsApiDefs.h"
#include "data/IBKRHisData.h"
#include "event/EventDispatcher.h"
#include "execution/BacktestHandler.h"
#include "portfolio/Portfolio.h"
#include "strategy/TestStrategy.h"

using namespace otterbot;
using namespace TwsApi;

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  FLAGS_log_dir = "/home/liuhao/workplace/logs";
  EventDispatcher dispatcher;
  IBKRHisData hisData(dispatcher, "AAPL");
  Portfolio portfolio(dispatcher, 1000000);
  TestStrategy strategy(dispatcher, "AAPL", 0.01);
  BacktestHandler backtest(dispatcher, portfolio);

  hisData.subscribe();
  while (!hisData.isEnd())
    ;
  std::cout << "P&L: " << portfolio.getPnL() << std::endl;
  return 0;
}