#include "execution/BacktestHandler.h"

#include "event/DataUpdateEvent.h"
#include "event/OrderEvent.h"
#include "event/OrderFillEvent.h"
#include "event/OrderRejectEvent.h"

namespace otterbot {

BacktestHandler::BacktestHandler(EventDispatcher& dispatcher,
                                 Portfolio& portfolio, bool short_enabled)
    : ExecutionHandler(dispatcher),
      portfolio_(portfolio),
      short_enabled_(short_enabled) {
  dispatcher_.register_handler(
      EventType::DataUpdate, [this](const Event& event) {
        this->onDataUpdate(dynamic_cast<const DataUpdateEvent&>(event));
      });

  dispatcher_.register_handler(EventType::Order, [this](const Event& event) {
    this->onOrder(dynamic_cast<const OrderEvent&>(event));
  });
}

void BacktestHandler::onOrder(const OrderEvent& order) {
  // check if a short sell while short_enalbed == false
  if (!short_enabled_ && order.getOrderSide() == OrderSide::SELL &&
      portfolio_.getPosition(order.getSymbol()) < order.getQuantity()) {
    OrderRejectEvent reject_event(order.getOrderId(), order.getSymbol(),
                                  order.getOrderType(), order.getOrderSide(),
                                  order.getPrice(), order.getQuantity(),
                                  "SHORT NOT ALLOWED");
    dispatcher_.dispatch(reject_event);
    return;
  }
  // check if enough buying power
  if (order.getOrderSide() == OrderSide::BUY &&
      portfolio_.getPower() < order.getPrice() * order.getQuantity()) {
    OrderRejectEvent reject_event(order.getOrderId(), order.getSymbol(),
                                  order.getOrderType(), order.getOrderSide(),
                                  order.getPrice(), order.getQuantity(),
                                  "INSUFFICIENT POWER");
    dispatcher_.dispatch(reject_event);
    return;
  }

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
        // Assuming no partially filled.
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