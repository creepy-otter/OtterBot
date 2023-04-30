#include "portfolio/Portfolio.h"

#include <mutex>

namespace otterbot {

Portfolio::Portfolio(EventDispatcher& dispatcher, double initial_cash)
    : dispatcher_(dispatcher), cash_(initial_cash), pnl_(initial_cash) {
  dispatcher_.register_handler(
      EventType::OrderFill, [this](const Event& event) {
        this->onOrderFill(dynamic_cast<const OrderFillEvent&>(event));
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
}

double Portfolio::getCash() const {
  std::shared_lock<std::shared_mutex> lock(mtx_);
  return cash_;
}

double Portfolio::getPnL() const {
  std::shared_lock<std::shared_mutex> lock(mtx_);
  return pnl_;
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
