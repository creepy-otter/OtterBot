#include "strategy/SMACrossStrategy.h"

#include <glog/logging.h>

#include <iomanip>

#include "event/EventType.h"
#include "lib/date.h"

namespace otterbot {
SMACrossStrategy::SMACrossStrategy(EventDispatcher& dispatcher, int fast,
                                   int slow)
    : Strategy(dispatcher), fast_(fast), slow_(slow) {}

/**
 * In this function, we need to calculate the moving average of the fast
 * period and the slow period. Then we need to compare the two moving average
 * values to determine whether to place a buy order or a sell order.
 */
void SMACrossStrategy::onDataUpdate(const DataUpdateEvent& event) {
  std::string symbol = event.getSymbol();
  double price = event.getPrice();
  auto timestamp = event.getTime();
  LOG(INFO) << "Received data update: " << symbol << " " << price << " "
            << date::format("%F %T", timestamp);
  if (fastMA_.find(symbol) == fastMA_.end()) {
    fastMA_.emplace(symbol, MA(fast_));
  }
  if (slowMA_.find(symbol) == slowMA_.end()) {
    slowMA_.emplace(symbol, MA(slow_));
  }
  fastMA_[symbol].update(price);
  slowMA_[symbol].update(price);
  if (fastMA_[symbol].isFilled() &&
      fastMA_[symbol].value() > slowMA_[symbol].value()) {
    if (position_[symbol] == 0) {
      placeBuyOrder(symbol, price);
    }
  } else if (fastMA_[symbol].isFilled() &&
             fastMA_[symbol].value() < slowMA_[symbol].value()) {
    if (position_[symbol] != 0) {
      placeSellOrder(symbol, price);
    }
  }
}

void SMACrossStrategy::onOrderFill(const OrderFillEvent& event) {
  std::string symbol = event.getSymbol();
  if (event.getOrderSide() == OrderSide::BUY) {
    position_[symbol] += event.getFillQuantity();
  } else {
    position_[symbol] -= event.getFillQuantity();
  }
}

void SMACrossStrategy::onOrderReject(const OrderRejectEvent& event) {
  LOG(INFO) << "Order Rejected: " << event.getMessage() << ", Buy "
            << event.getQuantity() << " of " << event.getSymbol() << " at "
            << event.getPrice();
}

OrderId SMACrossStrategy::placeBuyOrder(const std::string& symbol,
                                        double price) {
  double buy_price = price;
  LOG(INFO) << "Buy " << symbol << " at " << buy_price << " with quantity 1";
  return placeOrder(symbol, OrderType::LIMIT, OrderSide::BUY, buy_price, 1);
}

OrderId SMACrossStrategy::placeSellOrder(const std::string& symbol,
                                         double price) {
  double sell_price = price;
  LOG(INFO) << "Sell " << symbol << " at " << sell_price << " with quantity 1";
  return placeOrder(symbol, OrderType::LIMIT, OrderSide::SELL, sell_price, 1);
}
}  // namespace otterbot