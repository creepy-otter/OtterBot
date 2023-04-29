#pragma once

namespace otterbot {

enum class EventType {
  DataUpdate,
  OrderFill,
  Order,
  // Add more event types as needed
};

enum class OrderType { MARKET, LIMIT, STOP };

enum class OrderSide { BUY, SELL };

}  // namespace otterbot
