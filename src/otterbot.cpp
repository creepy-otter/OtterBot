#include <iomanip>
#include <iostream>

#include "data/CSVData.h"
#include "event/DataUpdateEvent.h"
#include "event/EventDispatcher.h"
#include "execution/BacktestHandler.h"
#include "portfolio/Portfolio.h"
#include "strategy/TestStrategy.h"

using namespace otterbot;

int main() {
  EventDispatcher dispatcher;
  CSVData csvData(dispatcher, "AAPL", "../OtterBot/data/aapl.csv");
  Portfolio portfolio(dispatcher, 100000);
  TestStrategy strategy(dispatcher, "AAPL", 0.02);
  BacktestHandler backtest(dispatcher, portfolio);

  // dispatcher.register_handler(
  //     EventType::DataUpdate, [&strategy](const Event& event) {
  //       strategy.onDataUpdate(dynamic_cast<const DataUpdateEvent&>(event));
  //     });

  // dispatcher.register_handler(
  //     EventType::DataUpdate, [&backtest](const Event& event) {
  //       backtest.onDataUpdate(dynamic_cast<const DataUpdateEvent&>(event));
  //     });

  // dispatcher.register_handler(
  //     EventType::Order, [&backtest](const Event& event) {
  //       backtest.onOrder(dynamic_cast<const OrderEvent&>(event));
  //     });

  // dispatcher.register_handler(
  //     EventType::OrderFill, [&strategy](const Event& event) {
  //       strategy.onOrderFill(dynamic_cast<const OrderFillEvent&>(event));
  //     });

  // dispatcher.register_handler(
  //     EventType::OrderFill, [&portfolio](const Event& event) {
  //       portfolio.onOrderFill(dynamic_cast<const OrderFillEvent&>(event));
  //     });

  while (!csvData.isEnd())
    ;
  std::cout << "P&L: " << portfolio.getPnL() << std::endl;
}