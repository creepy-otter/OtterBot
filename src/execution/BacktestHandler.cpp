#include "execution/BacktestHandler.h"

#include "event/DataUpdateEvent.h"
#include "event/OrderEvent.h"
#include "event/OrderFillEvent.h"
#include "event/OrderRejectEvent.h"

namespace otterbot {

BacktestHandler::BacktestHandler(EventDispatcher& dispatcher,
                                 Portfolio& portfolio)
    : ExecutionHandler(dispatcher), portfolio_(portfolio) {
  dispatcher_.register_handler(
      EventType::DataUpdate, [this](const Event& event) {
        this->onDataUpdate(dynamic_cast<const DataUpdateEvent&>(event));
      });

  dispatcher_.register_handler(EventType::Order, [this](const Event& event) {
    this->onOrder(dynamic_cast<const OrderEvent&>(event));
  });
}

void BacktestHandler::onOrder(const OrderEvent& order) {
  if (order.getOrderType() == OrderType::LIMIT) {
    order_map_[order.getSymbol()].push_back(order);
  } else {
    OrderFillEvent fill_event(order.getSymbol(), order.getOrderSide(),
                              order.getPrice(), order.getQuantity(),
                              order.getOrderId());
    dispatcher_.dispatch(fill_event);
  }
}

void BacktestHandler::onDataUpdate(const DataUpdateEvent& event) {
  const std::string& symbol = event.getSymbol();
  double price = event.getPrice();

  if (order_map_.count(symbol) > 0) {
    auto& orders = order_map_[symbol];
    for (size_t i = 0; i < orders.size();) {
      OrderEvent& order = orders[i];
      bool filled = false;
      if (order.getOrderType() == OrderType::LIMIT) {
        if (order.getOrderSide() == OrderSide::BUY && price <= order.getPrice())
          filled = true;
        else if (order.getOrderSide() == OrderSide::SELL &&
                 price >= order.getPrice())
          filled = true;
      } else if (order.getOrderType() == OrderType::STOP) {
        if (order.getOrderSide() == OrderSide::BUY && price >= order.getPrice())
          filled = true;
        else if (order.getOrderSide() == OrderSide::SELL &&
                 price <= order.getPrice())
          filled = true;
      }

      if (filled) {
        OrderFillEvent fill_event(symbol, order.getOrderSide(), price,
                                  order.getQuantity(), order.getOrderId());
        dispatcher_.dispatch(fill_event);

        std::swap(order, orders.back());
        orders.pop_back();
      } else {
        ++i;
      }
    }
  }
}

void BacktestHandler::onOrderFill() {}
}  // namespace otterbot