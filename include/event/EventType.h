#pragma once

#include <cstdint>

namespace otterbot {

enum class EventType {
  DataUpdate,
  OrderFill,
  Order,
  OrderReject
  // Add more event types as needed
};

enum class OrderType { MARKET, LIMIT, STOP };

enum class OrderSide { BUY, SELL };

using OrderId = uint64_t;

}  // namespace otterbot
