#include "strategy/TestStrategy.h"

#include <glog/logging.h>

#include <iomanip>
#include <iostream>

#include "event/OrderEvent.h"

namespace otterbot {

TestStrategy::TestStrategy(EventDispatcher& dispatcher,
                           const std::string& symbol, double distance)
    : Strategy(dispatcher),
      symbol_(symbol),
      distance_(distance),
      position_(0) {}

void TestStrategy::onDataUpdate(const DataUpdateEvent& event) {
  if (event.getSymbol() == symbol_) {
    double price = event.getPrice();
    if (position_ == 0) {
      placeBuyOrder(price);
      placeSellOrder(price);
    }
  }
}

void TestStrategy::onOrderFill(const OrderFillEvent& event) {
  if (event.getSymbol() == symbol_) {
    if (event.getOrderSide() == OrderSide::BUY) {
      position_ += 10;
      placeSellOrder(event.getFillPrice());
    } else if (event.getOrderSide() == OrderSide::SELL) {
      position_ -= 10;
      placeBuyOrder(event.getFillPrice());
    }
  }
  if (event.getOrderSide() == OrderSide::BUY) {
    LOG(INFO) << "Order Filled: Buy 10 at " << event.getFillPrice();
  } else if (event.getOrderSide() == OrderSide::SELL) {
    LOG(INFO) << "Order Filled: Sell 10 at " << event.getFillPrice();
  }
}

void TestStrategy::onOrderReject(const OrderRejectEvent& event) {
  if (event.getOrderSide() == OrderSide::BUY) {
    LOG(INFO) << "Order Rejected: " << event.getMessage() << ", Buy "
              << event.getQuantity() << " of " << event.getSymbol() << " at "
              << event.getPrice();
  } else if (event.getOrderSide() == OrderSide::SELL) {
    LOG(INFO) << "Order Rejected: " << event.getMessage() << ", Sell "
              << event.getQuantity() << " of " << event.getSymbol() << " at "
              << event.getPrice();
  }
}

OrderId TestStrategy::placeBuyOrder(double price) {
  double buy_price = price - distance_;
  return placeOrder(symbol_, OrderType::LIMIT, OrderSide::BUY, buy_price, 10);
}

OrderId TestStrategy::placeSellOrder(double price) {
  double sell_price = price + distance_;
  return placeOrder(symbol_, OrderType::LIMIT, OrderSide::SELL, sell_price, 10);
}

}  // namespace otterbot
