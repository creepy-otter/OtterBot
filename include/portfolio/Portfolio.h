#pragma once

#include <unordered_map>

#include "event/OrderFillEvent.h"

namespace otterbot {

class Portfolio {
 public:
  Portfolio(double initial_cash) : cash_(initial_cash), pnl_(0) {}

  void onOrderFill(const OrderFillEvent& event) {
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

  double getCash() const { return cash_; }
  double getPnL() const { return pnl_; }
  int getPosition(const std::string& symbol) const {
    auto it = positions_.find(symbol);
    if (it != positions_.end()) {
      return it->second;
    }
    return 0;
  }

 private:
  double cash_;
  double pnl_;
  std::unordered_map<std::string, int> positions_;
};
}  // namespace otterbot