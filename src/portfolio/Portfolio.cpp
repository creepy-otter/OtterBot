#include "portfolio/Portfolio.h"

namespace otterbot {

Portfolio::Portfolio(EventDispatcher& dispatcher, double initial_cash)
    : dispatcher_(dispatcher), cash_(initial_cash), pnl_(0) {
  dispatcher_.register_handler(
      EventType::OrderFill, [this](const Event& event) {
        this->onOrderFill(dynamic_cast<const OrderFillEvent&>(event));
      });
}

void Portfolio::onOrderFill(const OrderFillEvent& event) {
  const std::string& symbol = event.getSymbol();
  int quantity = event.getFillQuantity();
  double price = event.getFillPrice();

  if (event.getOrderSide() == OrderSide::BUY) {
    positions_[symbol] += quantity;
    cash_ -= quantity * price;
  } else if (event.getOrderSide() == OrderSide::SELL) {
    positions_[symbol] -= quantity;
    cash_ += quantity * price;
  }

  // Update P&L
  pnl_ = cash_;
  for (const auto& position : positions_) {
    // Assuming the price remains the same for simplicity
    pnl_ += position.second * price;
  }
}

double Portfolio::getCash() const { return cash_; }

double Portfolio::getPnL() const { return pnl_; }

int Portfolio::getPosition(const std::string& symbol) const {
  auto it = positions_.find(symbol);
  if (it != positions_.end()) {
    return it->second;
  }
  return 0;
}

}  // namespace otterbot
