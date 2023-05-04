#include "strategy/TestStrategy.h"

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
  // std::time_t t = std::chrono::system_clock::to_time_t(event.getTime());
  // std::cout << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S") << " "
  //           << event.getPrice() << std::endl;
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
  std::cout << "Order Filled: ";
  if (event.getOrderSide() == OrderSide::BUY)
    std::cout << "Buy  ";
  else if (event.getOrderSide() == OrderSide::SELL)
    std::cout << "Sell ";
  std::cout << "10 at " << event.getFillPrice() << std::endl;
  std::cout << "Current position of " << symbol_ << ": " << position_
            << std::endl;
}

void TestStrategy::onOrderReject(const OrderRejectEvent& event) {
  std::cout << "Order Rejected: " << event.getMessage() << ", ";
  if (event.getOrderSide() == OrderSide::BUY)
    std::cout << "Buy  ";
  else if (event.getOrderSide() == OrderSide::SELL)
    std::cout << "Sell ";
  std::cout << event.getQuantity() << " of " << event.getSymbol() << " at "
            << event.getPrice() << std::endl;
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
