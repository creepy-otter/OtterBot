#include "portfolio/Portfolio.h"

#include <mutex>

namespace otterbot {

Portfolio::Portfolio(EventDispatcher& dispatcher, double initial_cash)
    : dispatcher_(dispatcher),
      cash_(initial_cash),
      pnl_(initial_cash),
      power_(initial_cash) {
  dispatcher_.register_handler(
      EventType::OrderFill, [this](const Event& event) {
        this->onOrderFill(dynamic_cast<const OrderFillEvent&>(event));
      });
  dispatcher_.register_handler(EventType::Order, [this](const Event& event) {
    this->onOrder(dynamic_cast<const OrderEvent&>(event));
  });
  dispatcher_.register_handler(
      EventType::OrderReject, [this](const Event& event) {
        this->onOrderReject(dynamic_cast<const OrderRejectEvent&>(event));
      });
}

void Portfolio::onOrderFill(const OrderFillEvent& event) {
  std::unique_lock<std::shared_mutex> lock(mtx_);
  const std::string& symbol = event.getSymbol();
  int quantity = event.getFillQuantity();
  double price = event.getFillPrice();
  OrderSide side = event.getOrderSide();

  if (side == OrderSide::BUY) {
    positions_[symbol] += quantity;
  } else if (side == OrderSide::SELL) {
    positions_[symbol] -= quantity;
  }

  double cash_change = price * quantity * (side == OrderSide::BUY ? -1 : 1);
  cash_ += cash_change;
  // Update P&L
  pnl_ = cash_;
  for (const auto& position : positions_) {
    // Assuming the price remains the same for simplicity
    pnl_ += position.second * price;
  }
  // Update buying power
  if (side == OrderSide::SELL) {
    power_ += cash_change;
  }
}

void Portfolio::onOrder(const OrderEvent& event) {
  std::unique_lock<std::shared_mutex> lock(mtx_);
  if (event.getOrderSide() == OrderSide::BUY) {
    power_ -= event.getPrice() * event.getQuantity();
  }
}

void Portfolio::onOrderReject(const OrderRejectEvent& event) {
  std::unique_lock<std::shared_mutex> lock(mtx_);
  if (event.getOrderSide() == OrderSide::BUY) {
    power_ += event.getPrice() * event.getQuantity();
  }
}

double Portfolio::getCash() const {
  std::shared_lock<std::shared_mutex> lock(mtx_);
  return cash_;
}

double Portfolio::getPnL() const {
  std::shared_lock<std::shared_mutex> lock(mtx_);
  return pnl_;
}

double Portfolio::getPower() const {
  std::shared_lock<std::shared_mutex> lock(mtx_);
  return power_;
}

int Portfolio::getPosition(const std::string& symbol) const {
  std::shared_lock<std::shared_mutex> lock(mtx_);
  auto it = positions_.find(symbol);
  if (it != positions_.end()) {
    return it->second;
  }
  return 0;
}

}  // namespace otterbot
